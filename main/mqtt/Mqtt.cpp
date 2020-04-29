#include "Mqtt.h"
#include "../ModeController.h"
#include "../ConfigManager.h"
#include "../events.h"
#include "mqtt_client.h"
#include "esp_log.h"

#define LOG_TAG "MQTT"

void mqtt_event_connected(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    Mqtt* mqtt = static_cast<Mqtt*>(event_handler_arg);
    ESP_ERROR_CHECK(nullptr == mqtt ? ESP_FAIL : ESP_OK);

    mqtt->onMqttConnected();
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

Mqtt::Mqtt(ModeController *controller, ConfigManager *configManager):
    m_controller(controller), m_configManager(configManager)
{
    const std::string broker = configManager->getMqttBroker();

    esp_mqtt_client_config_t mqtt_cfg = {
            .uri = broker.c_str(),
    };
    m_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(m_client, MQTT_EVENT_CONNECTED, mqtt_event_connected, this);
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

void Mqtt::onMqttConnected()
{
    setupSubscriptions(m_configManager->getMqttDeviceTopic());
    setupSubscriptions(m_configManager->getMqttGroupTopic());
}

void Mqtt::onMqttData(esp_mqtt_event_handle_t event)
{
    if (nullptr != strstr(event->topic, "/power") && event->data_len >= 1) {
        m_controller->setPower(event->data[0] == '1');
    }
    if (nullptr != strstr(event->topic, "/mode") && event->data_len >= 1) {
        m_controller->setModeIndex(atoi(event->data));
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

        case LEDWALL_EVENT_MODE_CHANGED: {
            int *modeIndex = static_cast<int *>(event_data);
            publishState("mode", std::to_string(*modeIndex));
            break;
        }
    }
}

void Mqtt::setupSubscriptions(const std::string &baseTopic)
{
    std::string topic;

    topic = "/" + baseTopic + "/cmd/power";
    esp_mqtt_client_subscribe(m_client, topic.c_str(), 0);
    ESP_LOGI(LOG_TAG, "Subscribed to \"%s\"", topic.c_str());

    topic = "/" + baseTopic + "/cmd/mode";
    esp_mqtt_client_subscribe(m_client, topic.c_str(), 0);
    ESP_LOGI(LOG_TAG, "Subscribed to \"%s\"", topic.c_str());
}

void Mqtt::publishState(const std::string &state, const std::string &value)
{
    std::string topic = "/" + m_configManager->getMqttDeviceTopic() + "/state/" + state;

    esp_mqtt_client_publish(m_client, topic.c_str(), value.c_str(), 0, 0, 0);
}
