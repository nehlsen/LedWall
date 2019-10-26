#include "LedController.h"
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>
#include "FastLED.h"
#include "LedMode/LedModeStatus.h"
#include "LedMode/LedModeSample.h"

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

LedController::LedController()
{
    FastLED.addLeds<WS2812, CONFIG_DATA_PIN>(leds, CONFIG_NUM_LEDS);
    FastLED.setMaxPowerInVoltsAndMilliamps(5,1000);

    led_update_task_event_group = xEventGroupCreate();
    xEventGroupSetBits(led_update_task_event_group, LED_WALL_ENABLED_BIT);

    setMode(ModeStatus);

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
}

bool LedController::getPower() const
{
    return m_power;
}

void LedController::setMode(LedController::Mode mode)
{
    ESP_LOGI(LED_CONTROLLER_LOG_TAG, "setMode: %d", mode);

    LedMode *newMode = nullptr;
    switch (mode) {
        default:
            ESP_LOGE(LED_CONTROLLER_LOG_TAG, "UNRECOGNIZED MODE");
        case ModeStatus:
            newMode = new LedModeStatus(leds, CONFIG_NUM_LEDS);
            break;

        case ModeSample:
            newMode = new LedModeSample(leds, CONFIG_NUM_LEDS);
            break;
    }

    turnAllLedsOff();
    m_mode = mode;
    delete m_ledMode;
    m_ledMode = newMode;
    
    onChanged();
}

LedController::Mode LedController::getMode() const
{
    return m_mode;
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
