#include "MqttAdapter.h"
#include "../ModeController.h"
#include "../events.h"
#include <esp_log.h>
#include <cJSON.h>
#include "../LedMode/LedModes.h"
#include <Mqtt.h>
#include <MqttPublisher.h>

#define LOG_TAG "MQTT"

namespace LedWall {

void led_wall_events(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    auto mqtt = static_cast<MqttAdapter*>(event_handler_arg);
    ESP_ERROR_CHECK(nullptr == mqtt ? ESP_FAIL : ESP_OK);

    mqtt->onLedWallEvent(event_id, event_data);
}

MqttAdapter::MqttAdapter(ModeController *controller):
    m_controller(controller)
{
    esp_event_handler_register(LEDWALL_EVENTS, ESP_EVENT_ANY_ID, led_wall_events, this);

    auto mqtt = EBLi::Mqtt::instance();
    m_publisherPower = mqtt->createPublisher("power");
    m_publisherBrightness = mqtt->createPublisher("brightness");
    m_publisherModeIndex = mqtt->createPublisher("mode/index");
    m_publisherModeName = mqtt->createPublisher("mode/name");
    m_publisherModeOptions = mqtt->createPublisher("mode/options");
}

void MqttAdapter::onMqttData(esp_mqtt_event_handle_t event)
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

void MqttAdapter::onLedWallEvent(int32_t event_id, void *event_data)
{
    switch (event_id) {
        case LEDWALL_EVENT_POWER_CHANGED: {
            bool *isPowered = static_cast<bool *>(event_data);
            m_publisherPower->publishValue(*isPowered ? "1" : "0");
            break;
        }

        case LEDWALL_EVENT_BRIGHTNESS_CHANGED: {
            uint8_t *brightness = static_cast<uint8_t *>(event_data);
            m_publisherBrightness->publishValue(*brightness);
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

void MqttAdapter::publishMode()
{
    m_publisherModeIndex->publishValue(m_controller->getModeIndex());
    m_publisherModeName->publishValue(m_controller->getModeName());
}

void MqttAdapter::publishModeOptions()
{
    cJSON *optionsObject = cJSON_CreateObject();
    m_controller->getModeOptions(optionsObject);
    m_publisherModeOptions->publishValue(optionsObject);
    cJSON_Delete(optionsObject);
}

void MqttAdapter::setupSubscriptions(const std::string &baseTopic)
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


} // namespace LedWall
