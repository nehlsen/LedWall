#include "Network.h"
#include <sys/socket.h>

#define NET_LOG_TAG "LedMode:Network"
#define PORT 47011

static void udp_server_task(void *pvParameters)
{
    Network *net = static_cast<Network*>(pvParameters);

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
            ESP_LOGI(NET_LOG_TAG, "Waiting for data");
            int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);

            // Error occurred during receiving
            if (len < 0) {
                ESP_LOGE(NET_LOG_TAG, "recv failed: errno %d", errno);
                break;
            } else {
                // Data received
                ESP_LOGI(NET_LOG_TAG, "Received %d bytes", len);

                if (len >= 7 && net != nullptr) {
                    net->onNetworkData(rx_buffer, len);
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
    TaskHandle_t udpServerTask = nullptr; // TODO remember to stop it in DTOR
    xTaskCreate(udp_server_task, "udp_server", 4096, this, 5, &udpServerTask);
}

void Network::update()
{
    m_buffer.render(m_matrix);
}

void Network::onNetworkData(char *data, int length)
{
    if (length < 7) {
        return;
    }

    if (data[0] == 0x10) {
        handlePixelStreamData(data, length);
    }
}

void Network::handlePixelStreamData(char *data, int length)
{
    for (int pixel = 0; pixel < (length-2)/5; ++pixel) {
        uint8_t x = data[pixel*5 +2 +0];
        uint8_t y = data[pixel*5 +2 +1];
        uint8_t r = data[pixel*5 +2 +2];
        uint8_t g = data[pixel*5 +2 +3];
        uint8_t b = data[pixel*5 +2 +4];

        ESP_LOGI(NET_LOG_TAG, "pixel: %d,%d -> %d,%d,%d", x, y, r, g, b);

        m_buffer.merge(Pixel(x, y, CRGB(r, g, b)));
    }
}
