#include "Network.h"
#include <sys/socket.h>

namespace LedWall {
namespace Mode {

#define NET_LOG_TAG "LedMode:Network"
#define PORT 47011

struct PixelData {
    uint8_t x;
    uint8_t y;
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

static void readPixelStreamData(char *data, int length, QueueHandle_t receiveQueue)
{
    int bytesHandled = 0;

    for (int pixel = 0; pixel < (length-2)/5; ++pixel) {
        // 5 byte per pixel -> pixel * 5
        // 2 byte header    -> +2
        // actual data      -> +0 to +4

        PixelData pixelData = {
                (uint8_t)data[pixel*5 +2 +0],
                (uint8_t)data[pixel*5 +2 +1],
                (uint8_t)data[pixel*5 +2 +2],
                (uint8_t)data[pixel*5 +2 +3],
                (uint8_t)data[pixel*5 +2 +4]
        };
        if (errQUEUE_FULL == xQueueSendToBack(receiveQueue, &pixelData, 0)) {
            ESP_LOGW(NET_LOG_TAG, "Failed to write to Queue! Queue full!");
        } else {
            bytesHandled += 5;
        }
    }

    ESP_LOGI(NET_LOG_TAG, "readPixelStreamData, handled %d of %d bytes", bytesHandled, length);
}

static void readNetworkData(char *data, int length, QueueHandle_t receiveQueue)
{
    if (length < 7) {
        return;
    }

    if (data[0] == 0x10) {
        readPixelStreamData(data, length, receiveQueue);
    }
}

static void udp_server_task(void *pvParameters)
{
    QueueHandle_t receiveQueue = static_cast<QueueHandle_t>(pvParameters);

    char rx_buffer[128];
    int addr_family;
    int ip_protocol;

    while (true) {
        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;

        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(NET_LOG_TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(NET_LOG_TAG, "Socket created");

        int err = bind(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err < 0) {
            ESP_LOGE(NET_LOG_TAG, "Socket unable to bind: errno %d", errno);
        }
        ESP_LOGI(NET_LOG_TAG, "Socket bound, port %d", PORT);

        while (true) {
            ESP_LOGD(NET_LOG_TAG, "Waiting for data");
            int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);

            // Error occurred during receiving
            if (len < 0) {
                ESP_LOGE(NET_LOG_TAG, "recv failed: errno %d", errno);
                break;
            } else {
                // Data received
                ESP_LOGD(NET_LOG_TAG, "Received %d bytes", len);

                if (len >= 7 && receiveQueue) {
                    readNetworkData(rx_buffer, len, receiveQueue);
                }
            }
        }

        if (sock != -1) {
            ESP_LOGE(NET_LOG_TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}

Network::Network(LedMatrix &matrix) : LedMode(matrix)
{
    m_receiveQueue = xQueueCreate(matrix.getWidth()*matrix.getHeight(), sizeof(PixelData));
    if (m_receiveQueue == nullptr) {
        ESP_LOGE(NET_LOG_TAG, "Failed to create Queue!");
    } else {
        ESP_LOGI(NET_LOG_TAG, "Queue created, starting udp-server task...");
        xTaskCreate(udp_server_task, "udp_server", 4096, m_receiveQueue, 5, &m_udpServerTask);
    }
}

Network::~Network()
{
    vTaskDelete(m_udpServerTask);
}

bool Network::update()
{
    UBaseType_t cnt = uxQueueMessagesWaiting(m_receiveQueue);
    if (cnt > 0)
        ESP_LOGI(NET_LOG_TAG, "update, %d messages on queue", cnt);

    if (readQueue()) {
        m_buffer.render(m_matrix);
        return true;
    }

    return false;
}

bool Network::readQueue()
{
    if (m_receiveQueue == nullptr) {
        ESP_LOGW(NET_LOG_TAG, "Queue not ready!");
        return false;
    }

    uint8_t countNewPixels = 0;
    auto *readBuffer = new PixelData;
    Pixels newPixels;
    do {
        if(xQueueReceive(m_receiveQueue, readBuffer, 0)) {
            newPixels.push_back(Pixel(readBuffer->x, readBuffer->y, CRGB(readBuffer->r, readBuffer->g, readBuffer->b)));

            ++countNewPixels;
        } else {
            // queue is empty. stop for now
            break;
        }
    } while (true);
    delete readBuffer;

    m_buffer.merge(newPixels);
    if (countNewPixels > 0) ESP_LOGI(NET_LOG_TAG, "readQueue, received %d new pixels", countNewPixels);

    return countNewPixels > 0;
}

} // namespace Mode
} // namespace LedWall
