#include "MqttAdapter.h"
#include "../ModeController.h"
#include "../events.h"
#include "../LedMode/LedModes.h"
#include <Mqtt.h>
#include <esp_log.h>
#include <cJSON.h>

#define LOG_TAG "MqttAdapter"

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

    setupPublishers();
    setupSubscribers();
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

void MqttAdapter::setupPublishers()
{
    auto mqtt = EBLi::Mqtt::instance();

    m_publisherPower = mqtt->createPublisher("power", EBLi::MqttPublisher::RetainValue);
    m_publisherBrightness = mqtt->createPublisher("brightness");
    m_publisherModeIndex = mqtt->createPublisher("mode/index");
    m_publisherModeName = mqtt->createPublisher("mode/name");
    m_publisherModeOptions = mqtt->createPublisher("mode/options");
}

void MqttAdapter::setupSubscribers()
{
    auto mqtt = EBLi::Mqtt::instance();

    mqtt->createSubscriber("power", [this](const std::string &value) {
        m_controller->setPower(value == "1");
    });
    mqtt->createSubscriber("brightness", [this](const std::string &value) {
        m_controller->setBrightness(std::stoi(value));
    });
    mqtt->createSubscriber("mode/index", [this](const std::string &value) {
        m_controller->setModeByIndex(std::stoi(value));
    });
    mqtt->createSubscriber("mode/name", [this](const std::string &value) {
        m_controller->setModeByName(value);
    });
    mqtt->createSubscriber("mode/options", [this](const std::string &value) {
        cJSON *optionsObject = cJSON_Parse(value.c_str());
        m_controller->setModeOptions(optionsObject);
        cJSON_Delete(optionsObject);
    });
    mqtt->createSubscriber("preset/save", [this](const std::string &value) {
        m_controller->savePreset(value);
    });
    mqtt->createSubscriber("preset/load", [this](const std::string &value) {
        m_controller->loadPreset(value);
    });
    mqtt->createSubscriber("preset/delete", [this](const std::string &value) {
        m_controller->deletePreset(value);
    });
    mqtt->createSubscriber("preset/clear", [this](const std::string &value) {
        m_controller->deleteAllPresets();
    });
    mqtt->createSubscriber("reboot", [this](const std::string &value) {
        m_controller->triggerSystemReboot();
    });
}

} // namespace LedWall
