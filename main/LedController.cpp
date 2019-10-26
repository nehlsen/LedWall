#include "LedController.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "FastLED.h"

CRGB leds[CONFIG_NUM_LEDS];

void led_task(void *pvParameters)
{
    ESP_LOGI("LED_WALL", "led_task..");

    int COLOR_DELAY = 200;

    for ( ;; ) {
        for (int i = 0; i < CONFIG_NUM_LEDS; ++i) {
            leds[i] = CRGB::White;
            FastLED.show();
            vTaskDelay(COLOR_DELAY / portTICK_RATE_MS);

            leds[i] = CRGB::Red;
            FastLED.show();
            vTaskDelay(COLOR_DELAY / portTICK_RATE_MS);

            leds[i] = CRGB::Green;
            FastLED.show();
            vTaskDelay(COLOR_DELAY / portTICK_RATE_MS);

            leds[i] = CRGB::Blue;
            FastLED.show();
            vTaskDelay(COLOR_DELAY / portTICK_RATE_MS);

            leds[i] = CRGB::Black;
            FastLED.show();
//            vTaskDelay(COLOR_DELAY / portTICK_RATE_MS);
        }
    }
}

LedController::LedController()
{
    FastLED.addLeds<WS2812, CONFIG_DATA_PIN>(leds, CONFIG_NUM_LEDS);
    FastLED.setMaxPowerInVoltsAndMilliamps(5,1000);

    xTaskCreatePinnedToCore(&led_task, "led_task", 4000, nullptr, 5, nullptr, 0);
}

void LedController::setPower(bool power)
{
    m_power = power;
    onChanged();
}

bool LedController::getPower()
{
    return m_power;
}

void LedController::setMode(LedController::Mode mode)
{
    m_mode = mode;
    onChanged();
}

LedController::Mode LedController::getMode()
{
    return m_mode;
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
