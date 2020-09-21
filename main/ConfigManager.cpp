#include <esp_log.h>
#include <Mqtt.h>
#include <ConfigManager.h>
#include "ConfigManager.h"

namespace LedWall {

// NOTE keys are limited to 15 characters
#define MATRIX_WIDTH_KEY        "matrix_width"
#define MATRIX_HEIGHT_KEY       "matrix_height"
#define MATRIX_BRIGHTNESS_KEY   "matrix_brghtnss"
#define RESTART_COUNTER_KEY     "restart_counter"
#define POWER_LAST_STATE_KEY    "power_state"
#define POWER_BOOT_MODE_KEY     "power_boot_into"
#define LED_MODE_LAST_STATE_KEY "led_mode_state"
#define LED_MODE_BOOT_MODE_KEY  "led_mode_boot"

uint8_t ConfigManager::getMatrixWidth() const
{
    return EBLi::ConfigManager::instance()->getValue(MATRIX_WIDTH_KEY, 4);
}

void ConfigManager::setMatrixWidth(uint8_t width)
{
    EBLi::ConfigManager::instance()->setValue(MATRIX_WIDTH_KEY, width);
}

uint8_t ConfigManager::getMatrixHeight() const
{
    return EBLi::ConfigManager::instance()->getValue(MATRIX_HEIGHT_KEY, 4);
}

void ConfigManager::setMatrixHeight(uint8_t height)
{
    EBLi::ConfigManager::instance()->setValue(MATRIX_HEIGHT_KEY, height);
}

uint8_t ConfigManager::getBrightness() const
{
    return EBLi::ConfigManager::instance()->getValue(MATRIX_BRIGHTNESS_KEY, 255);
}

void ConfigManager::setBrightness(uint8_t brightness)
{
    EBLi::ConfigManager::instance()->setValue(MATRIX_BRIGHTNESS_KEY, brightness);
}

void ConfigManager::setPowerOnResetMode(ConfigManager::AutoPowerOn mode)
{
    EBLi::ConfigManager::instance()->setValue(POWER_BOOT_MODE_KEY, mode);
}

ConfigManager::AutoPowerOn ConfigManager::getPowerOnResetMode() const
{
    return (AutoPowerOn)EBLi::ConfigManager::instance()->getValue(POWER_BOOT_MODE_KEY, RECOVER_LAST);
}

void ConfigManager::setPowerState(bool currentPowerState)
{
    EBLi::ConfigManager::instance()->setValue(POWER_LAST_STATE_KEY, currentPowerState);
}

bool ConfigManager::isPoweredOnBoot() const
{
    auto mode = getPowerOnResetMode();
    switch (mode) {
        case ALWAYS_OFF:
            return false;

        case ALWAYS_ON:
            return true;

        default:
        case RECOVER_LAST:
            return EBLi::ConfigManager::instance()->getValue(POWER_LAST_STATE_KEY, 1); // FIXME move to getter?
    }
}

void ConfigManager::setLedModeAutoRestore(int autoRestoreMode)
{
    EBLi::ConfigManager::instance()->setValue(LED_MODE_BOOT_MODE_KEY, autoRestoreMode);
}

int ConfigManager::getLedModeAutoRestore() const
{
    return EBLi::ConfigManager::instance()->getValue(LED_MODE_BOOT_MODE_KEY, -1);
}

void ConfigManager::setLedMode(int currentModeIndex)
{
    EBLi::ConfigManager::instance()->setValue(LED_MODE_LAST_STATE_KEY, currentModeIndex);
}

int ConfigManager::getBootIntoMode() const
{
    auto ledModeAutoRestore = getLedModeAutoRestore();

    if (ledModeAutoRestore < 0) {
        // restore last mode, fallback to mode "0"
        return EBLi::ConfigManager::instance()->getValue(LED_MODE_LAST_STATE_KEY, 0);
    }

    return ledModeAutoRestore;
}

std::string ConfigManager::getMqttBroker() const
{
    return EBLi::Mqtt::instance()->getBroker();
}

void ConfigManager::setMqttBroker(const std::string& brokerUrl)
{
    EBLi::Mqtt::instance()->setBroker(brokerUrl);
}

std::string ConfigManager::getMqttDeviceTopic() const
{
    return EBLi::Mqtt::instance()->getDeviceTopic();
}

void ConfigManager::setMqttDeviceTopic(const std::string& topic)
{
    EBLi::Mqtt::instance()->setDeviceTopic(topic);
}

std::string ConfigManager::getMqttGroupTopic() const
{
    return EBLi::Mqtt::instance()->getGroupTopic();
}

void ConfigManager::setMqttGroupTopic(const std::string& topic)
{
    EBLi::Mqtt::instance()->setGroupTopic(topic);
}

} // namespace LedWall
