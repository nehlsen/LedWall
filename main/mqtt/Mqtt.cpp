#include "Mqtt.h"
#include "../ModeController.h"
#include "../ConfigManager.h"
#include "../events.h"
#include "mqtt_client.h"
#include "esp_log.h"
#include <cJSON.h>
#include "../LedMode/LedModes.h"

#define LOG_TAG "MQTT"

namespace LedWall {

void mqtt_event_connected(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    Mqtt* mqtt = static_cast<Mqtt*>(event_handler_arg);
    ESP_ERROR_CHECK(nullptr == mqtt ? ESP_FAIL : ESP_OK);

    mqtt->onMqttConnected();
}

void mqtt_event_disconnected(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    Mqtt* mqtt = static_cast<Mqtt*>(event_handler_arg);
    ESP_ERROR_CHECK(nullptr == mqtt ? ESP_FAIL : ESP_OK);

    mqtt->onMqttDisconnected();
}

void mqtt_event_data(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    Mqtt* mqtt = static_cast<Mqtt*>(event_handler_arg);
    ESP_ERROR_CHECK(nullptr == mqtt ? ESP_FAIL : ESP_OK);

    mqtt->onMqttData((esp_mqtt_event_handle_t)event_data);
}

void led_wall_events(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    Mqtt* mqtt = static_cast<Mqtt*>(event_handler_arg);
    ESP_ERROR_CHECK(nullptr == mqtt ? ESP_FAIL : ESP_OK);

    mqtt->onLedWallEvent(event_id, event_data);
}

/* ****************************************************************************************************************** */

Mqtt::Mqtt(ModeController *controller, ConfigManager *configManager):
    m_controller(controller), m_configManager(configManager), m_isConnected(false)
{
    const std::string broker = configManager->getMqttBroker();

    esp_mqtt_client_config_t mqtt_cfg = {
            .uri = broker.c_str(),
    };
    m_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(m_client, MQTT_EVENT_CONNECTED, mqtt_event_connected, this);
    esp_mqtt_client_register_event(m_client, MQTT_EVENT_DISCONNECTED, mqtt_event_disconnected, this);
    esp_mqtt_client_register_event(m_client, MQTT_EVENT_DATA, mqtt_event_data, this);

    esp_event_handler_register(LEDWALL_EVENTS, ESP_EVENT_ANY_ID, led_wall_events, this);
}

void Mqtt::start()
{
    esp_err_t err = esp_mqtt_client_start(m_client);
    if (err != ESP_OK) {
        ESP_LOGW(LOG_TAG, "MQTT Start Error: \"%s\"", esp_err_to_name(err));
        return;
    }
    ESP_LOGI(LOG_TAG, "MQTT Started.");
}

void Mqtt::stop()
{
    esp_mqtt_client_stop(m_client);
    ESP_LOGI(LOG_TAG, "MQTT Stopped.");
}

bool Mqtt::isConnected() const
{
    return m_isConnected;
}

void Mqtt::onMqttConnected()
{
    setupSubscriptions(m_configManager->getMqttDeviceTopic());
    setupSubscriptions(m_configManager->getMqttGroupTopic());

    m_isConnected = true;

    ESP_LOGI(LOG_TAG, "MQTT Connection established...");
}

void Mqtt::onMqttDisconnected()
{
    m_isConnected = false;

    ESP_LOGI(LOG_TAG, "MQTT Connection lost...");
}

void Mqtt::onMqttData(esp_mqtt_event_handle_t event)
{
    if (nullptr != strstr(event->topic, "/power") && event->data_len >= 1) {
        m_controller->setPower(event->data[0] == '1');
    }
    if (nullptr != strstr(event->topic, "/brightness") && event->data_len >= 1) {
        m_controller->setBrightness(atoi(event->data));
    }
    if (nullptr != strstr(event->topic, "/mode/index") && event->data_len >= 1) {
        m_controller->setModeByIndex(atoi(event->data));
    }
    if (nullptr != strstr(event->topic, "/mode/name") && event->data_len >= 1) {
        if (strlen(event->data) <= Mode::ValidModeNameLength) {
            m_controller->setModeByName(event->data);
        }
    }
    if (nullptr != strstr(event->topic, "/mode/options") && event->data_len >= 1) {
        cJSON *optionsObject = cJSON_Parse(event->data);
        m_controller->setModeOptions(optionsObject);
        cJSON_Delete(optionsObject);
    }
    if (nullptr != strstr(event->topic, "/reboot")) {
        m_controller->triggerSystemReboot();
    }
}

void Mqtt::onLedWallEvent(int32_t event_id, void *event_data)
{
    switch (event_id) {
        case LEDWALL_EVENT_POWER_CHANGED: {
            bool *isPowered = static_cast<bool *>(event_data);
            publishState("power", *isPowered ? "1" : "0");
            break;
        }

        case LEDWALL_EVENT_BRIGHTNESS_CHANGED: {
            uint8_t *brightness = static_cast<uint8_t *>(event_data);
            publishState("brightness", std::to_string(*brightness));
            break;
        }

        case LEDWALL_EVENT_MODE_CHANGED:
            publishMode();
            publishModeOptions();
            break;

        case LEDWALL_EVENT_MODE_OPTIONS_CHANGED:
            publishModeOptions();
            break;
    }
}

void Mqtt::publishMode()
{
    publishState("mode/index", std::to_string(m_controller->getModeIndex()));
    publishState("mode/name", m_controller->getModeName());
}

void Mqtt::publishModeOptions()
{
    cJSON *optionsObject = cJSON_CreateObject();
    m_controller->getModeOptions(optionsObject);

    char *jsonBuffer = cJSON_PrintUnformatted(optionsObject);
    publishState("mode/options", jsonBuffer);

    free(jsonBuffer);
    cJSON_Delete(optionsObject);
}

void Mqtt::setupSubscriptions(const std::string &baseTopic)
{
    auto subscribeToCommand = [baseTopic, this](const std::string &command) {
        std::string topic = "/" + baseTopic + "/cmd/" + command;
        esp_mqtt_client_subscribe(m_client, topic.c_str(), 0);
        ESP_LOGI(LOG_TAG, "Subscribed to \"%s\"", topic.c_str());
    };

    subscribeToCommand("power");
    subscribeToCommand("reboot");
    subscribeToCommand("brightness");
    subscribeToCommand("mode/index");
    subscribeToCommand("mode/name");
    subscribeToCommand("mode/options");
}

void Mqtt::publishState(const std::string &state, const std::string &value)
{
    if (!isConnected()) {
        return;
    }

    std::string topic = "/" + m_configManager->getMqttDeviceTopic() + "/state/" + state;

    esp_mqtt_client_publish(m_client, topic.c_str(), value.c_str(), 0, 0, 0);
}

} // namespace LedWall
