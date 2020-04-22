#include <esp_log.h>
#include "ConfigManager.h"

#define NVS_NAMESPACE "led_wall_config"
static const char *CONFIG_MANAGER_LOG_TAG = "CONFIG_MANAGER";

#define MATRIX_WIDTH_KEY "matrix_width"
#define MATRIX_HEIGHT_KEY "matrix_height"
#define MATRIX_BRIGHTNESS_KEY "matrix_brightness"
#define RESTART_COUNTER_KEY "restart_counter"
#define POWER_LAST_STATE_KEY "power_state"
#define POWER_BOOT_MODE_KEY "power_boot_into"
#define LED_MODE_LAST_STATE_KEY "led_mode_state"
#define LED_MODE_BOOT_MODE_KEY "led_mode_boot_into"

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

ConfigManager::AutoPowerOn ConfigManager::getPowerOnResetMode()
{
    return (AutoPowerOn)getIntVal(POWER_BOOT_MODE_KEY, RECOVER_LAST);
}

void ConfigManager::setPowerState(bool currentPowerState)
{
    setIntVal(POWER_LAST_STATE_KEY, currentPowerState);
}

bool ConfigManager::isPoweredOnBoot()
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

int ConfigManager::getLedModeAutoRestore()
{
    return getIntVal(LED_MODE_BOOT_MODE_KEY, -1);
}

void ConfigManager::setLedMode(int currentModeIndex)
{
    setIntVal(LED_MODE_LAST_STATE_KEY, currentModeIndex);
}

int ConfigManager::getBootIntoMode()
{
    auto ledModeAutoRestore = getLedModeAutoRestore();

    if (ledModeAutoRestore < 0) {
        // restore last mode, fallback to mode "0"
        return getIntVal(LED_MODE_LAST_STATE_KEY, 0);
    }

    return ledModeAutoRestore;
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
