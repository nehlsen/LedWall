#include "ModeController.h"
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>
#include <LedMatrix.h>
#include <algorithm>
#include <cJSON.h>
#include "LedMode/LedModes.h"
#include "ConfigManager.h"
#include "ModeOptionsPersister.h"
#include "events.h"

namespace LedWall {

static const char *LOG_TAG = "ModeController";

#define LED_WALL_ENABLED_BIT BIT0
static TaskHandle_t led_update_task_hdnl;
static EventGroupHandle_t led_update_task_event_group;

void led_update_task(void *pvParameter)
{
    ESP_LOGI(LOG_TAG, "led_update_task...");

    auto *controller = static_cast<ModeController*>(pvParameter);
    ESP_ERROR_CHECK(nullptr == controller ? ESP_FAIL : ESP_OK);

    while (true) {
        random16_add_entropy(random());

        Mode::LedMode *ledMode = controller->getLedMode();
        if (ledMode) {
            if (ledMode->update()) {
                FastLED.show();
            }
        }

        vTaskDelay(ledMode->frameDelay());
        xEventGroupWaitBits(led_update_task_event_group, LED_WALL_ENABLED_BIT, false, false, portMAX_DELAY);
    }
}

ModeController::ModeController(ConfigManager *configManager):
    m_configManager(configManager)
{
    const int matrixWidth = m_configManager->getMatrixWidth();
    const int matrixHeight = m_configManager->getMatrixHeight();
    ESP_LOGI(LOG_TAG, "matrixWidth: %d, matrixHeight: %d, total LEDs: %d", matrixWidth, matrixHeight, (matrixWidth * matrixHeight));
    ESP_LOGD(LOG_TAG, "memory/LED: %d, total memory: %d", (int)sizeof(CRGB), (matrixWidth * matrixHeight) * (int)sizeof(CRGB));
    ESP_LOGD(LOG_TAG, "Free memory (before alloc): %d bytes", esp_get_free_heap_size());
    m_leds = (CRGB*)malloc((matrixWidth*matrixHeight) * sizeof(CRGB));
    ESP_ERROR_CHECK(nullptr == m_leds ? ESP_ERR_NO_MEM : ESP_OK);
    ESP_LOGD(LOG_TAG, "Free memory (after alloc): %d bytes", esp_get_free_heap_size());

    ESP_LOGI(LOG_TAG, "Using PIN %d for LEDs", CONFIG_DATA_PIN);
    auto &fastLedController = CFastLED::addLeds<WS2812, CONFIG_DATA_PIN, GRB>(m_leds, (matrixWidth*matrixHeight));
//    fastLedController.setCorrection(TypicalLEDStrip);

//    m_matrix = new LedMatrix(fastLedController, matrixWidth, matrixHeight, MatrixInvertHorizontal);
    m_matrix = new LedMatrix(FastLED, matrixWidth, matrixHeight);

    turnAllLedsOff();

    led_update_task_event_group = xEventGroupCreate();
    xEventGroupSetBits(led_update_task_event_group, LED_WALL_ENABLED_BIT);

    setPower(m_configManager->isPoweredOnBoot());
    setBrightness(m_configManager->getBrightness());
    setModeByIndex(m_configManager->getBootIntoMode());

    xTaskCreatePinnedToCore(
            &led_update_task,
            "led_update_task",
            4000,
            this,
            5,
            &led_update_task_hdnl,
            0
            );
}

void ModeController::setPower(bool power)
{
    ESP_LOGI(LOG_TAG, "setPower: %d", power);

    m_power = power;
    if (!m_power) {
        turnAllLedsOff();
    }
    setLedUpdateTaskEnabled(m_power);
    
    m_configManager->setPowerState(m_power);
    esp_event_post(LEDWALL_EVENTS, LEDWALL_EVENT_POWER_CHANGED, (void*)&m_power, sizeof(m_power), portMAX_DELAY);
}

bool ModeController::getPower() const
{
    return m_power;
}

void ModeController::triggerSystemReboot()
{
    ESP_LOGI(LOG_TAG, "SYSTEM REBOOT TRIGGERED!");
    turnAllLedsOff();
    esp_restart(); // FIXME give e.g. http time to send a response!
}

void ModeController::setBrightness(uint8_t brightness)
{
    ESP_LOGI(LOG_TAG, "Set Brightness %.0f%%", (brightness/255.0)*100.0);
    m_matrix->setBrightness(brightness);

    m_configManager->setBrightness(brightness);
    esp_event_post(LEDWALL_EVENTS, LEDWALL_EVENT_BRIGHTNESS_CHANGED, (void*)&brightness, sizeof(brightness), portMAX_DELAY);
}

uint8_t ModeController::getBrightness() const
{
    return m_matrix->getBrightness();
}

bool ModeController::setModeByName(const std::string &name)
{
    auto it = std::find_if(Mode::LedModes.begin(), Mode::LedModes.end(), [name](const Mode::LedModeDef_t &modeDef) {
        return modeDef.name == name;
    });

    if (it != Mode::LedModes.end()) {
        return setModeByIndex(distance(Mode::LedModes.begin(), it));
    }

    return false;
}

bool ModeController::setModeByIndex(int modeIndex)
{
    ESP_LOGI(LOG_TAG, "setModeByIndex: modeIndex:%d", modeIndex);

    if (modeIndex < 0 || modeIndex >= Mode::LedModes.size()) {
        ESP_LOGE(LOG_TAG, "setModeByIndex: Failed to set Mode: Invalid Index");
        return false;
    }

    ESP_LOGI(LOG_TAG, "setModeByIndex: going to create mode:\"%s\"", Mode::LedModes.at(modeIndex).name);
    Mode::LedMode *newMode = Mode::LedModes.at(modeIndex).factory(*m_matrix);

    if (nullptr != m_ledMode) {
        ModeOptionsPersister::save(m_ledMode, Mode::LedModes.at(m_modeIndex).name);
    }
    ModeOptionsPersister::load(newMode, Mode::LedModes.at(modeIndex).name);

    turnAllLedsOff();
    m_modeIndex = modeIndex;
    delete m_ledMode;
    m_ledMode = newMode;

    m_configManager->setLedMode(m_modeIndex);
    esp_event_post(LEDWALL_EVENTS, LEDWALL_EVENT_MODE_CHANGED, (void*)&m_modeIndex, sizeof(m_modeIndex), portMAX_DELAY);

    return true;
}

int ModeController::getModeIndex() const
{
    return m_modeIndex;
}

bool ModeController::setModeOptions(cJSON *optionsObject)
{
    if (nullptr == getLedMode()) {
        return false;
    }
    if (nullptr == optionsObject) {
        return false;
    }

    bool optionsHaveBeenSet = getLedMode()->writeOptions(optionsObject);
    if (optionsHaveBeenSet) {
        ModeOptionsPersister::save(m_ledMode, Mode::LedModes.at(m_modeIndex).name);
        esp_event_post(LEDWALL_EVENTS, LEDWALL_EVENT_MODE_OPTIONS_CHANGED, nullptr, 0, portMAX_DELAY);
    }

    return optionsHaveBeenSet;
}

void ModeController::getModeOptions(cJSON *optionsObject)
{
    if (nullptr == getLedMode()) {
        return;
    }
    if (nullptr == optionsObject) {
        return;
    }

    getLedMode()->readOptions(optionsObject);
}

Mode::LedMode* ModeController::getLedMode() const
{
    return m_ledMode;
}

void ModeController::setLedUpdateTaskEnabled(bool enabled)
{
    if (!led_update_task_hdnl) {
        return;
    }
    
    if (enabled) {
        xEventGroupSetBits(led_update_task_event_group, LED_WALL_ENABLED_BIT);
    } else {
        xEventGroupClearBits(led_update_task_event_group, LED_WALL_ENABLED_BIT);
    }
}

void ModeController::turnAllLedsOff()
{
    m_matrix->clear();
}

} // namespace LedWall
