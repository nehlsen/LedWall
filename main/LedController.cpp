#include "LedController.h"
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>
#include "FastLED.h"
#include "LedMode/LedModes.h"
#include "ConfigManager.h"

static const char *LED_CONTROLLER_LOG_TAG = "LED_CONTROLLER";

#define LED_WALL_ENABLED_BIT BIT0
static TaskHandle_t led_update_task_hdnl;
static EventGroupHandle_t led_update_task_event_group;

CRGB leds[CONFIG_NUM_LEDS];

void led_update_task(void *pvParameter)
{
    ESP_LOGI(LED_CONTROLLER_LOG_TAG, "led_update_task...");

    auto *controller = (LedController*)pvParameter;

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

LedController::LedController(ConfigManager *configManager):
    m_configManager(configManager)
{
    FastLED.addLeds<WS2812, CONFIG_DATA_PIN>(leds, CONFIG_NUM_LEDS).setCorrection(TypicalLEDStrip);
//    FastLED.setBrightness(MAX_BRIGHTNESS);
    FastLED.setMaxPowerInVoltsAndMilliamps(5,1000);
    FastLED.clear();

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

void LedController::setPower(bool power)
{
    ESP_LOGI(LED_CONTROLLER_LOG_TAG, "setPower: %d", power);

    m_power = power;
    if (!m_power) {
        turnAllLedsOff();
    }
    setLedUpdateTaskEnabled(m_power);
    
    onChanged();
    m_configManager->setPowerState(m_power);
}

bool LedController::getPower() const
{
    return m_power;
}

bool LedController::setModeIndex(int modeIndex)
{
    ESP_LOGI(LED_CONTROLLER_LOG_TAG, "setModeIndex: modeIndex:%d", modeIndex);

    if (modeIndex < 0 || modeIndex >= LedModes.size()) {
        ESP_LOGE(LED_CONTROLLER_LOG_TAG, "setModeIndex: Failed to set Mode: Invalid Index");
        return false;
    }

    ESP_LOGI(LED_CONTROLLER_LOG_TAG, "setModeIndex: going to create mode:\"%s\"", LedModes.at(modeIndex).name);
    LedMode *newMode = LedModes.at(modeIndex).factory(leds, CONFIG_NUM_LEDS);

    turnAllLedsOff();
    m_modeIndex = modeIndex;
    delete m_ledMode;
    m_ledMode = newMode;
    
    onChanged();
    m_configManager->setLedMode(m_modeIndex);
    return true;
}

int LedController::getModeIndex() const
{
    return m_modeIndex;
}

LedMode* LedController::getLedMode() const
{
    return m_ledMode;
}

void LedController::setChangeHandler(change_handler_t change_handler)
{
    m_change_handler = change_handler;
}

void LedController::onChanged()
{
    if (m_change_handler == nullptr) {
        return;
    }

    m_change_handler();
}

void LedController::setLedUpdateTaskEnabled(bool enabled)
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

void LedController::turnAllLedsOff()
{
    FastLED.showColor(CRGB::Black);
}
