#include <mdns.h>
#include <esp_spiffs.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "tcpip_adapter.h"
#include "esp_log.h"

#include "wifi_connector.h"
#include "LedController.h"
#include "mqtt.h"
#include "WebServer.h"
#include "ConfigManager.h"

static const char *APP_LOG_TAG = "LED_WALL";

#ifdef __cplusplus
extern "C" {
#endif

static void initialise_mdns()
{
    if (mdns_init() != ESP_OK) {
        ESP_LOGW(APP_LOG_TAG, "mDNS init failed");
    }

    mdns_hostname_set(CONFIG_MDNS_HOSTNAME);
    mdns_instance_name_set("LedWall REST and Web interface");

    mdns_txt_item_t serviceTxtData[] = {
            {"board", "esp32"},
            {"path", "/"}
    };

    ESP_ERROR_CHECK(mdns_service_add("LedWall-WebServer", "_http", "_tcp", 80, serviceTxtData,
                                     sizeof(serviceTxtData) / sizeof(serviceTxtData[0])));
}

esp_err_t init_fs(void)
{
    esp_vfs_spiffs_conf_t conf = {
            .base_path = "",
            .partition_label = nullptr,
            .max_files = 5,
            .format_if_mount_failed = false
    };
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(APP_LOG_TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(APP_LOG_TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(APP_LOG_TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return ESP_FAIL;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(nullptr, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(APP_LOG_TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(APP_LOG_TAG, "Partition size: total: %d, used: %d", total, used);
    }
    return ESP_OK;
}

void app_main()
{
    ESP_LOGI(APP_LOG_TAG, "[APP] Startup..");
    ESP_LOGI(APP_LOG_TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(APP_LOG_TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set(APP_LOG_TAG, ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_CONNECTOR", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    initialise_mdns();
    ESP_ERROR_CHECK(wifi_connector_start());
    ESP_ERROR_CHECK(init_fs());

    ConfigManager *cfg = new ConfigManager;
    cfg->open();
    LedController *controller = new LedController(cfg);
    mqtt_app_start(controller);
    // TODO the example creates (starts) the server once wifi is ready, stops and restarts on re-connects
    WebServer *server = new WebServer(controller, cfg);
    server->startServer();
}

#ifdef __cplusplus
} // extern "C"
#endif
