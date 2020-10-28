#include <ebli.h>
#include <mdns.h>
#include <esp_log.h>
#include <OtaUpdater.h>
#include "ModeController.h"
#include "ConfigManager.h"
#include "Config.h"

#if defined(CONFIG_ENABLE_REST)
#include "WebServer.h"
#endif

#if defined(CONFIG_ENABLE_MQTT)
#include "mqtt/MqttAdapter.h"
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

void app_main()
{
    EBLi::init_all();
    initialise_mdns();

    LedWall::Config::init();

    auto controller = new LedWall::ModeController;

    auto updater = new EBLi::OtaUpdater;

    #if defined(CONFIG_ENABLE_MQTT)
    auto mqtt = new LedWall::MqttAdapter(controller);
    #endif

    #if defined(CONFIG_ENABLE_REST)
    auto *server = new LedWall::WebServer(controller, updater);
    #endif
}

#ifdef __cplusplus
} // extern "C"
#endif
