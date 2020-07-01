#include <esp_log.h>
#include "ConfigManager.h"

namespace LedWall {

#define NVS_NAMESPACE "led_wall_config"
static const char *CONFIG_MANAGER_LOG_TAG = "ConfigManager";

// NOTE keys are limited to 15 characters
#define MATRIX_WIDTH_KEY        "matrix_width"
#define MATRIX_HEIGHT_KEY       "matrix_height"
#define MATRIX_BRIGHTNESS_KEY   "matrix_brghtnss"
#define RESTART_COUNTER_KEY     "restart_counter"
#define POWER_LAST_STATE_KEY    "power_state"
#define POWER_BOOT_MODE_KEY     "power_boot_into"
#define LED_MODE_LAST_STATE_KEY "led_mode_state"
#define LED_MODE_BOOT_MODE_KEY  "led_mode_boot"

#define MQTT_BROKER_KEY         "mqtt_broker"
#define MQTT_DEVICE_TOPIC_KEY   "mqtt_device_tpc"
#define MQTT_GROUP_TOPIC_KEY    "mqtt_group_tpc"

#define MQTT_BROKER_DEFAULT "mqtt://iot.eclipse.org"
#define MQTT_DEVICE_TOPIC_DEFAULT "a-led-wall"
#define MQTT_GROUP_TOPIC_DEFAULT "ledwalls"

bool ConfigManager::open()
{
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &m_nvsHandle);
 
    if (err != ESP_OK) {
        ESP_LOGE(CONFIG_MANAGER_LOG_TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return false;
    }
    
    updateRestartCounter();
    return true;
}

void ConfigManager::close()
{
    nvs_close(m_nvsHandle);
}

void ConfigManager::setAutoCommitEnabled(bool enabled)
{
    m_autoCommitEnabled = enabled;
}

bool ConfigManager::isAutoCommitEnabled() const
{
    return m_autoCommitEnabled;
}

bool ConfigManager::commit()
{
    esp_err_t err = nvs_commit(m_nvsHandle);
    
    if (err != ESP_OK) {
        ESP_LOGE(CONFIG_MANAGER_LOG_TAG, "Error (%s) committing NVS Data!", esp_err_to_name(err));
        return false;
    }

    return true;
}

int32_t ConfigManager::getIntVal(const char *key, int32_t defaultValue) const
{
    int32_t value = defaultValue;
    esp_err_t err = nvs_get_i32(m_nvsHandle, key, &value);

    if (err != ESP_OK) {
        ESP_LOGW(CONFIG_MANAGER_LOG_TAG, "NVS read Error (%s), key:\"%s\"", esp_err_to_name(err), key);
    }
    
    return value;
}

bool ConfigManager::setIntVal(const char *key, int32_t value)
{
    esp_err_t err = nvs_set_i32(m_nvsHandle, key, value);

    if (err != ESP_OK) {
        ESP_LOGE(CONFIG_MANAGER_LOG_TAG, "NVS write Error (%s), key:\"%s\"", esp_err_to_name(err), key);
        return false;
    }

    return autoCommit();
}

std::string ConfigManager::getStringVal(const char *key, const std::string &defaultValue) const
{
    size_t required_size;
    esp_err_t err = nvs_get_str(m_nvsHandle, key, nullptr, &required_size);
    if (err != ESP_OK) {
        ESP_LOGW(CONFIG_MANAGER_LOG_TAG, "NVS read Error (%s), key:\"%s\"", esp_err_to_name(err), key);
        return defaultValue;
    }
    if (required_size >= 64) {
        ESP_LOGW(CONFIG_MANAGER_LOG_TAG, "NVS read failed - max size exceeded, key:\"%s\"", key);
        return defaultValue;
    }

    char* value = (char*)malloc(required_size);
    err = nvs_get_str(m_nvsHandle, key, value, &required_size);
    if (err != ESP_OK) {
        ESP_LOGW(CONFIG_MANAGER_LOG_TAG, "NVS read Error (%s), key:\"%s\"", esp_err_to_name(err), key);
        return defaultValue;
    }

    auto ret = std::string(value);
    free(value);
    return ret;
}

bool ConfigManager::setStringVal(const char *key, const std::string &value)
{
    esp_err_t err = nvs_set_str(m_nvsHandle, key, value.c_str());

    if (err != ESP_OK) {
        ESP_LOGE(CONFIG_MANAGER_LOG_TAG, "NVS write Error (%s), key:\"%s\"", esp_err_to_name(err), key);
        return false;
    }

    return autoCommit();
}

int32_t ConfigManager::getRestartCounter()
{
    if (m_restartCounter == 0) {
        m_restartCounter = getIntVal(RESTART_COUNTER_KEY, 0);
    }
    
    return m_restartCounter;
}

uint8_t ConfigManager::getMatrixWidth() const
{
    return getIntVal(MATRIX_WIDTH_KEY, 4);
}

void ConfigManager::setMatrixWidth(uint8_t width)
{
    setIntVal(MATRIX_WIDTH_KEY, width);
}

uint8_t ConfigManager::getMatrixHeight() const
{
    return getIntVal(MATRIX_HEIGHT_KEY, 4);
}

void ConfigManager::setMatrixHeight(uint8_t height)
{
    setIntVal(MATRIX_HEIGHT_KEY, height);
}

uint8_t ConfigManager::getBrightness() const
{
    return getIntVal(MATRIX_BRIGHTNESS_KEY, 255);
}

void ConfigManager::setBrightness(uint8_t brightness)
{
    setIntVal(MATRIX_BRIGHTNESS_KEY, brightness);
}

void ConfigManager::setPowerOnResetMode(ConfigManager::AutoPowerOn mode)
{
    setIntVal(POWER_BOOT_MODE_KEY, mode);
}

ConfigManager::AutoPowerOn ConfigManager::getPowerOnResetMode() const
{
    return (AutoPowerOn)getIntVal(POWER_BOOT_MODE_KEY, RECOVER_LAST);
}

void ConfigManager::setPowerState(bool currentPowerState)
{
    setIntVal(POWER_LAST_STATE_KEY, currentPowerState);
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
            return getIntVal(POWER_LAST_STATE_KEY, 1); // FIXME move to getter?
    }
}

void ConfigManager::setLedModeAutoRestore(int autoRestoreMode)
{
    setIntVal(LED_MODE_BOOT_MODE_KEY, autoRestoreMode);
}

int ConfigManager::getLedModeAutoRestore() const
{
    return getIntVal(LED_MODE_BOOT_MODE_KEY, -1);
}

void ConfigManager::setLedMode(int currentModeIndex)
{
    setIntVal(LED_MODE_LAST_STATE_KEY, currentModeIndex);
}

int ConfigManager::getBootIntoMode() const
{
    auto ledModeAutoRestore = getLedModeAutoRestore();

    if (ledModeAutoRestore < 0) {
        // restore last mode, fallback to mode "0"
        return getIntVal(LED_MODE_LAST_STATE_KEY, 0);
    }

    return ledModeAutoRestore;
}

std::string ConfigManager::getMqttBroker() const
{
    return getStringVal(MQTT_BROKER_KEY, MQTT_BROKER_DEFAULT);
}

void ConfigManager::setMqttBroker(const std::string& brokerUrl)
{
    setStringVal(MQTT_BROKER_KEY, brokerUrl);
}

std::string ConfigManager::getMqttDeviceTopic() const
{
    return getStringVal(MQTT_DEVICE_TOPIC_KEY, MQTT_DEVICE_TOPIC_DEFAULT);
}

void ConfigManager::setMqttDeviceTopic(const std::string& topic)
{
    setStringVal(MQTT_DEVICE_TOPIC_KEY, topic);
}

std::string ConfigManager::getMqttGroupTopic() const
{
    return getStringVal(MQTT_GROUP_TOPIC_KEY, MQTT_GROUP_TOPIC_DEFAULT);
}

void ConfigManager::setMqttGroupTopic(const std::string& topic)
{
    setStringVal(MQTT_GROUP_TOPIC_KEY, topic);
}

bool ConfigManager::autoCommit()
{
    if (!isAutoCommitEnabled()) {
        return true;
    }
    
    return commit();
}

void ConfigManager::updateRestartCounter()
{
    if (m_restartCounter > 0) {
        // already incremented...
        return;
    }

    int32_t newCount = getRestartCounter() + 1;
    if (setIntVal(RESTART_COUNTER_KEY, newCount)) {
        ESP_LOGI(CONFIG_MANAGER_LOG_TAG, "Restart counter updated to %d", newCount);
        m_restartCounter = newCount;
    }
}

} // namespace LedWall
