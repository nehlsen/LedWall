#include "wifi_provisioning.h"

#include <string.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <wifi_provisioning/manager.h>
#include <wifi_provisioning/scheme_ble.h>

#ifdef __cplusplus
extern "C" {
#endif

static const char *PROVISIONING_SERVICE_PREFIX = "PROV_";
static const char *WIFI_PROVISIONING_LOG_TAG = "WIFI_PROVISIONING";

/* Event handler for catching provisioning manager events */
static void prov_event_handler(void *user_data, wifi_prov_cb_event_t event, void *event_data)
{
    switch (event) {
        case WIFI_PROV_START:
            ESP_LOGI(WIFI_PROVISIONING_LOG_TAG, "Provisioning started");
            break;
        case WIFI_PROV_CRED_RECV: {
            wifi_sta_config_t *wifi_sta_cfg = (wifi_sta_config_t *)event_data;
            /* If SSID length is exactly 32 bytes, null termination
             * will not be present, so explicitly obtain the length */
            size_t ssid_len = strnlen((const char *)wifi_sta_cfg->ssid, sizeof(wifi_sta_cfg->ssid));
            ESP_LOGI(WIFI_PROVISIONING_LOG_TAG,
                    "Received Wi-Fi credentials\n\tSSID     : %.*s\n\tPassword : %s",
                     ssid_len, (const char *) wifi_sta_cfg->ssid,
                     (const char *) wifi_sta_cfg->password
                     );
            break;
        }
        case WIFI_PROV_CRED_FAIL: {
            wifi_prov_sta_fail_reason_t *reason = (wifi_prov_sta_fail_reason_t *)event_data;
            ESP_LOGE(WIFI_PROVISIONING_LOG_TAG,
                    "Provisioning failed!\n\tReason : %s\n\tPlease reset to factory and retry provisioning",
                     (*reason == WIFI_PROV_STA_AUTH_ERROR) ? "Wi-Fi AP password incorrect" : "Wi-Fi AP not found"
                     );
            break;
        }
        case WIFI_PROV_CRED_SUCCESS:
            ESP_LOGI(WIFI_PROVISIONING_LOG_TAG, "Provisioning successful");
            break;
        case WIFI_PROV_END:
            /* De-initialize manager once provisioning is finished */
            wifi_prov_mgr_deinit();
            break;
        default:
            break;
    }
}

static void wifi_init_sta()
{
    ESP_LOGI(WIFI_PROVISIONING_LOG_TAG, "Starting Wifi Station...");
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());
}

static void on_got_ip(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    ESP_LOGI(WIFI_PROVISIONING_LOG_TAG, "Got IP: IPv4 address: " IPSTR, IP2STR(&event->ip_info.ip));
}

static void on_wifi_disconnect(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    ESP_LOGI(WIFI_PROVISIONING_LOG_TAG, "Wi-Fi disconnected, trying to reconnect...");
    ESP_ERROR_CHECK(esp_wifi_connect());
}

static void prov_service_name(char *service_name, size_t max)
{
    uint8_t eth_mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, eth_mac);
    snprintf(service_name, max, "%s%02X%02X%02X", PROVISIONING_SERVICE_PREFIX, eth_mac[3], eth_mac[4], eth_mac[5]);
}

static void start_provisioning(void)
{
    ESP_LOGI(WIFI_PROVISIONING_LOG_TAG, "Starting provisioning");

    char service_name[12]; // advertised ble device name
    prov_service_name(service_name, sizeof(service_name));

    /* This step is only useful when scheme is wifi_prov_scheme_ble. This will
     * set a custom 128 bit UUID which will be included in the BLE advertisement
     * and will correspond to the primary GATT service that provides provisioning
     * endpoints as GATT characteristics. Each GATT characteristic will be
     * formed using the primary service UUID as base, with different auto assigned
     * 12th and 13th bytes (assume counting starts from 0th byte). The client side
     * applications must identify the endpoints by reading the User Characteristic
     * Description descriptor (0x2901) for each characteristic, which contains the
     * endpoint name of the characteristic */
    uint8_t custom_service_uuid[] = {
            /* LSB <---------------------------------------
             * ---------------------------------------> MSB */
            0x21, 0x43, 0x65, 0x87, 0x09, 0xba, 0xdc, 0xfe,
            0xef, 0xcd, 0xab, 0x90, 0x78, 0x56, 0x34, 0x12
    };
    wifi_prov_scheme_ble_set_service_uuid(custom_service_uuid);

    /* Start provisioning service */
    ESP_ERROR_CHECK(wifi_prov_mgr_start_provisioning(WIFI_PROV_SECURITY_1, CONFIG_PROVISION_POP, service_name, NULL));
}

esp_err_t wifi_provisioning_start(void)
{
    /* Initialize Wi-Fi */
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &on_wifi_disconnect, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &on_got_ip, NULL));

    wifi_prov_mgr_config_t config = {
            .scheme = wifi_prov_scheme_ble,
            .scheme_event_handler = WIFI_PROV_SCHEME_BLE_EVENT_HANDLER_FREE_BTDM, // FIXME keep BLE?!

            /* Do we want an application specific handler be executed on
             * various provisioning related events */
            .app_event_handler = {
                    .event_cb = prov_event_handler,
                    .user_data = NULL
            }
    };

    /* Initialize provisioning manager with the
     * configuration parameters set above */
    ESP_ERROR_CHECK(wifi_prov_mgr_init(config));

    bool provisioned = false;
    /* Let's find out if the device is provisioned */
    ESP_ERROR_CHECK(wifi_prov_mgr_is_provisioned(&provisioned));

    /* If device is not yet provisioned start provisioning service */
    if (!provisioned) {
        start_provisioning();
    } else {
        ESP_LOGI(WIFI_PROVISIONING_LOG_TAG, "Already provisioned, starting Wi-Fi STA");

        /* We don't need the manager as device is already provisioned,
         * so let's release it's resources */
        wifi_prov_mgr_deinit();

        /* Start Wi-Fi station */
        wifi_init_sta();
    }

    return ESP_OK;
}

#ifdef __cplusplus
} // extern "C"
#endif
