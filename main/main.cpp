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
    ESP_ERROR_CHECK(wifi_connector_start());

    ConfigManager *cfg = new ConfigManager;
    cfg->open();
    LedController *controller = new LedController(cfg);
    mqtt_app_start(controller);
    // TODO the example creates (starts) the server once wifi is ready, stops and restarts on re-connects
    WebServer *server = new WebServer(controller);
    server->startServer();
}

#ifdef __cplusplus
} // extern "C"
#endif
