#include "Bars.h"

#include <cJSON.h>

uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Bars::update()
{
    for (int i = 0; i < m_ledCount; i++) {
        m_leds[i].nscale8(m_fadeRate);
    }

    uint16_t delay = map(m_barsRate, 0, 255, 0, 1000);
    int64_t currentTime = esp_timer_get_time() / 1000;
    if (currentTime - m_lastBarTime > delay || currentTime < 1) {
        draw();
        advanceDrawMode();
        m_lastBarTime = currentTime;
    }

    vTaskDelay(20 / portTICK_PERIOD_MS);
}

void Bars::readOptions(cJSON *root)
{
    cJSON_AddNumberToObject(root, "fadeRate", m_fadeRate);
    cJSON_AddNumberToObject(root, "barsRate", m_barsRate);
}

bool Bars::writeOptions(cJSON *root)
{
    cJSON *const requestedFadeRate = cJSON_GetObjectItem(root, "fadeRate");
    if (requestedFadeRate) {
        int rate = requestedFadeRate->valueint;
        m_fadeRate = constrain(rate, 0, 255);
    }

    cJSON *const requestedBarsRate = cJSON_GetObjectItem(root, "barsRate");
    if (requestedBarsRate) {
        int rate = requestedBarsRate->valueint;
        m_barsRate = constrain(rate, 0, 255);
    }

    return requestedFadeRate || requestedBarsRate;
}

void Bars::draw()
{
    switch (m_drawMode) {
        case DrawVertical:
            drawVerticalBar(m_lastBarAt);
            break;
        case DrawHorizontal:
            drawHorizontalBar(m_lastBarAt);
            break;
        case DrawDiagonalBl:
            drawDiagonalBarBl(m_lastBarAt);
            break;
        case DrawDiagonalBr:
            drawDiagonalBarBr(m_lastBarAt);
            break;
    }
}

void Bars::advanceDrawMode()
{
    m_lastBarAt++;

    switch (m_drawMode) {
        case DrawVertical:
            if (m_lastBarAt >= CONFIG_NUM_LEDS_HORIZONTAL) {
                m_drawMode = DrawHorizontal;
                m_lastBarAt = 0;
            }
            break;
        case DrawHorizontal:
            if (m_lastBarAt >= CONFIG_NUM_LEDS_VERTICAL) {
                m_drawMode = DrawDiagonalBl;
                m_lastBarAt = 0;
            }
            break;
        case DrawDiagonalBl:
            if (m_lastBarAt >= CONFIG_NUM_LEDS_HORIZONTAL + CONFIG_NUM_LEDS_VERTICAL - 1) {
                m_drawMode = DrawDiagonalBr;
                m_lastBarAt = 0;
            }
            break;
        case DrawDiagonalBr:
            if (m_lastBarAt >= CONFIG_NUM_LEDS_HORIZONTAL + CONFIG_NUM_LEDS_VERTICAL - 1) {
                m_drawMode = DrawVertical;
                m_lastBarAt = 0;
            }
            break;
    }
}

void Bars::drawVerticalBar(uint8_t x)
{
    uint8_t randomHue = random8();

    for (uint8_t y = 0; y < CONFIG_NUM_LEDS_VERTICAL; ++y) {
        m_leds[XY(x, y)].setHSV(randomHue, 255, 255);
    }
}

void Bars::drawHorizontalBar(uint8_t y)
{
    uint8_t randomHue = random8();

    for (uint8_t x = 0; x < CONFIG_NUM_LEDS_HORIZONTAL; ++x) {
        m_leds[XY(x, y)].setHSV(randomHue, 255, 255);
    }
}

void Bars::drawDiagonalBarBl(uint8_t frame)
{
    uint8_t randomHue = random8();

    for (uint8_t y = 0; y < CONFIG_NUM_LEDS_VERTICAL; ++y) {
        m_leds[XY(frame - y, y)].setHSV(randomHue, 255, 255);
    }
}

void Bars::drawDiagonalBarBr(uint8_t frame)
{
    uint8_t randomHue = random8();

    for (uint8_t y = 0; y < CONFIG_NUM_LEDS_VERTICAL; ++y) {
        m_leds[XY(CONFIG_NUM_LEDS_HORIZONTAL - frame - 1 + y, y)].setHSV(randomHue, 255, 255);
    }
}
