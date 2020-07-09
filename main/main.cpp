#include <mdns.h>
#include <esp_spiffs.h>
#include "nvs_flash.h"
#include "esp_event.h"
#include "tcpip_adapter.h"
#include "esp_log.h"
#include "wifi_provisioning.h"
#include "ModeController.h"
#include "ConfigManager.h"
#include "OtaUpdater.h"

#ifdef CONFIG_ENABLE_REST
#pragma message "REST enabled"
#include "WebServer.h"
#endif

#ifdef CONFIG_ENABLE_MQTT
#pragma message "MQTT enabled"
#include "mqtt/Mqtt.h"
#endif

ESP_EVENT_DEFINE_BASE(LEDWALL_EVENTS);

static const char *APP_LOG_TAG = "LED_WALL";

#ifdef __cplusplus
extern "C" {
#endif

static esp_err_t initialise_mdns()
{
    esp_err_t ret = mdns_init();
    if (ret != ESP_OK) {
        ESP_LOGW(APP_LOG_TAG, "mDNS: init failed");
        return ret;
    }

    ret = mdns_hostname_set(CONFIG_MDNS_HOSTNAME);
    if (ret != ESP_OK) {
        ESP_LOGW(APP_LOG_TAG, "mDNS: set hostname failed");
        return ret;
    }

    ret = mdns_instance_name_set("LedWall REST and Web interface");
    if (ret != ESP_OK) {
        ESP_LOGW(APP_LOG_TAG, "mDNS: set instance name failed");
        return ret;
    }

    mdns_txt_item_t httpServiceTxt[] = {
            {"board", "esp32"},
            {"path", "/"}
    };

    ESP_ERROR_CHECK(mdns_service_add("LedWall-WebServer", "_http", "_tcp", 80, httpServiceTxt,
                                     sizeof(httpServiceTxt) / sizeof(httpServiceTxt[0])));
    ESP_ERROR_CHECK(mdns_service_add("LedWall-MQTT", "_mqtt", "_tcp", 1883, {}, 0));

    return ESP_OK;
}

esp_err_t init_fs(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW(APP_LOG_TAG, "NVS: No Free Pages or new NVS Version. Erasing and retrying...");
        ret = nvs_flash_erase();
        ret |= nvs_flash_init();
    }
    if (ret != ESP_OK) {
        ESP_LOGE(APP_LOG_TAG, "NVS: Failed to initialize NVS");
        return ret;
    }
    ESP_LOGI(APP_LOG_TAG, "NVS: Init complete");

    esp_vfs_spiffs_conf_t conf = {
            .base_path = "",
            .partition_label = nullptr,
            .max_files = 5,
            .format_if_mount_failed = false
    };
    ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(APP_LOG_TAG, "SPIFFS: Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(APP_LOG_TAG, "SPIFFS: Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(APP_LOG_TAG, "SPIFFS: Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return ESP_FAIL;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(nullptr, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(APP_LOG_TAG, "SPIFFS: Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(APP_LOG_TAG, "SPIFFS: Partition size: total: %d, used: %d", total, used);
    }
    return ESP_OK;
}

void app_main()
{
    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set(APP_LOG_TAG, ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_CONNECTOR", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(init_fs());
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    initialise_mdns();
    ESP_ERROR_CHECK(wifi_provisioning_start()); // asnync / non-blocking! The old wifi WAS blocking!

    auto cfg = new LedWall::ConfigManager;
    cfg->open();
    auto controller = new LedWall::ModeController(cfg);

    auto updater = new LedWall::OtaUpdater;

    #ifdef CONFIG_ENABLE_MQTT
    auto mqtt = new LedWall::Mqtt(controller, cfg);
    #endif

    #ifdef CONFIG_ENABLE_REST
    // TODO the example creates (starts) the server once wifi is ready, stops and restarts on re-connects
    auto *server = new LedWall::WebServer(controller, cfg, updater);
    server->startServer();
    #endif

//    FIXME factory reset using push button required - could be solved using ESP IDF config
}

#ifdef __cplusplus
} // extern "C"
#endif
