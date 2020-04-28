#include "ModeController.h"
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>
#include <LedMatrix.h>
#include "LedMode/LedModes.h"
#include "ConfigManager.h"

static const char *LOG_TAG = "ModeController";

#define LED_WALL_ENABLED_BIT BIT0
static TaskHandle_t led_update_task_hdnl;
static EventGroupHandle_t led_update_task_event_group;

void led_update_task(void *pvParameter)
{
    ESP_LOGI(LOG_TAG, "led_update_task...");

    auto *controller = (ModeController*)pvParameter;

//    int delay = (1000/ledMode->fps()) / portTICK_PERIOD_MS;
    int delay = (1000/25) / portTICK_PERIOD_MS;

    while (true) {
        LedMode *ledMode = controller->getLedMode();
        if (ledMode) ledMode->update();

        FastLED.show();

        vTaskDelay(delay);
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
    auto &fastLedController = CFastLED::addLeds<WS2812, CONFIG_DATA_PIN>(m_leds, (matrixWidth*matrixHeight));
//    fastLedController.setCorrection(TypicalLEDStrip);
    ESP_LOGI(LOG_TAG, "Setting Brightness to %d%%", (m_configManager->getBrightness()/255)*100);
    FastLED.setBrightness(m_configManager->getBrightness());

    m_matrix = new LedMatrix(fastLedController, matrixWidth, matrixHeight, MatrixInvertHorizontal);

    turnAllLedsOff();

    led_update_task_event_group = xEventGroupCreate();
    xEventGroupSetBits(led_update_task_event_group, LED_WALL_ENABLED_BIT);

    setPower(m_configManager->isPoweredOnBoot());
    setModeIndex(m_configManager->getBootIntoMode());

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
    
    onChanged();
    m_configManager->setPowerState(m_power);
}

bool ModeController::getPower() const
{
    return m_power;
}

bool ModeController::setModeIndex(int modeIndex)
{
    ESP_LOGI(LOG_TAG, "setModeIndex: modeIndex:%d", modeIndex);

    if (modeIndex < 0 || modeIndex >= LedModes.size()) {
        ESP_LOGE(LOG_TAG, "setModeIndex: Failed to set Mode: Invalid Index");
        return false;
    }

    ESP_LOGI(LOG_TAG, "setModeIndex: going to create mode:\"%s\"", LedModes.at(modeIndex).name);
    LedMode *newMode = LedModes.at(modeIndex).factory(*m_matrix);

    turnAllLedsOff();
    m_modeIndex = modeIndex;
    delete m_ledMode;
    m_ledMode = newMode;
    
    onChanged();
    m_configManager->setLedMode(m_modeIndex);
    return true;
}

int ModeController::getModeIndex() const
{
    return m_modeIndex;
}

LedMode* ModeController::getLedMode() const
{
    return m_ledMode;
}

void ModeController::setChangeHandler(change_handler_t change_handler)
{
    m_change_handler = change_handler;
}

void ModeController::onChanged()
{
    if (m_change_handler == nullptr) {
        return;
    }

    m_change_handler();
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
    FastLED.clear(true);
    FastLED.delay(1);
}
