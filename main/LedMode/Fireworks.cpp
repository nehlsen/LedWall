#include "Fireworks.h"

#include <cJSON.h>
#define MAX_INT_VALUE 65536

void Fireworks::update()
{
    uint8_t randomHue = random8();
    uint16_t randomLed = random16(); // not really the LED but a control value to choose a LED

    for (int i = 0; i < FastLED.size(); i++) {
        FastLED.leds()[i].nscale8(m_fadeRate);
    }

    if (randomLed < (MAX_INT_VALUE / (256 - (constrain(m_sparkRate, 1, 256))))) {
        m_leds[randomLed % m_ledCount].setHSV(randomHue, 255, 255);
    }

    vTaskDelay(20 / portTICK_PERIOD_MS);
}

void Fireworks::readOptions(cJSON *root)
{
    cJSON_AddNumberToObject(root, "fadeRate", m_fadeRate);
    cJSON_AddNumberToObject(root, "sparkRate", m_sparkRate);
}

bool Fireworks::writeOptions(cJSON *root)
{
    cJSON *const requestedFadeRate = cJSON_GetObjectItem(root, "fadeRate");
    if (requestedFadeRate) {
        int rate = requestedFadeRate->valueint;
        m_fadeRate = constrain(rate, 0, 255);
    }

    cJSON *const requestedSparkRate = cJSON_GetObjectItem(root, "sparkRate");
    if (requestedSparkRate) {
        int rate = requestedSparkRate->valueint;
        m_sparkRate = constrain(rate, 0, 255);
    }

    return requestedFadeRate || requestedSparkRate;
}
