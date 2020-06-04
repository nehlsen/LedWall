#include "Breathe.h"
#include <Circle.h>
#include <cJSON.h>

namespace LedWall {
namespace Mode {

Breathe::Breathe(LedMatrix &matrix) : LedMode(matrix)
{
//    const int x = m_matrix.getWidth() / 2 - 1;
//    const int y = m_matrix.getHeight() / 2 - 1;
//    const int radius = (x < y ? x : y);
//    m_circle.setCenter(x, y).setRadius(radius);
//    m_circle.setBorderColor(m_color).setFillColor(m_color);

    m_shape.setStart(0, 0).setSize(m_matrix.getWidth(), m_matrix.getHeight());
    m_shape.setBorderColor(m_color).setFillColor(m_color);
}

bool Breathe::update()
{
    const double reciprocal_of_e = 0.367879441; // 1 / e

    float val = (exp(sin(millis() / (m_cycleTime) * 2 * PI)) - reciprocal_of_e) * m_brightnessVariation + m_offset;

    CRGB color(m_color);
    color.nscale8(255*val);
//    ESP_LOGI("Breathe", "val %f / %d,%d,%d", val, color.r, color.g, color.b);

    m_shape.setBorderColor(color).setFillColor(color);
    m_shape.render(m_matrix);

    return true;
}

void Breathe::readOptions(cJSON *root)
{
    // TODO shape
    auto *color = cJSON_AddObjectToObject(root, "color");
    cJSON_AddNumberToObject(color, "r", m_color.r);
    cJSON_AddNumberToObject(color, "g", m_color.g);
    cJSON_AddNumberToObject(color, "b", m_color.b);
    cJSON_AddNumberToObject(root, "variation", getVariation());
    cJSON_AddNumberToObject(root, "cycleTime", m_cycleTime);
}

bool Breathe::writeOptions(cJSON *root)
{
    cJSON *const requestedColor = cJSON_GetObjectItem(root, "color");
    if (requestedColor) {
        uint8_t r = 0, g = 0, b = 0;

        cJSON *colorValue = cJSON_GetObjectItem(requestedColor, "r");
        if (colorValue) {
            r = constrain(colorValue->valueint, 0, 255);
        }
        colorValue = cJSON_GetObjectItem(requestedColor, "g");
        if (colorValue) {
            g = constrain(colorValue->valueint, 0, 255);
        }
        colorValue = cJSON_GetObjectItem(requestedColor, "b");
        if (colorValue) {
            b = constrain(colorValue->valueint, 0, 255);
        }

        setColor(CRGB(r, g, b));
    }

    cJSON *const requestedVariation = cJSON_GetObjectItem(root, "variation");
    if (requestedVariation) {
        setVariation(constrain(requestedVariation->valueint, 1, 100));
    }

    cJSON *const requestedCycleTime = cJSON_GetObjectItem(root, "cycleTime");
    if (requestedCycleTime) {
        setCycleTime(constrain(requestedCycleTime->valueint, 1000, 60000));
    }

    return requestedColor || requestedVariation || requestedCycleTime;
}

void Breathe::setColor(const CRGB &color)
{
    m_color = color;
}

void Breathe::setVariation(uint8_t percent)
{
    static const double euler = 2.71828;

    if (percent < 1 || percent >= 100) {
        return;
    }

    const double variation = percent / 100.0;
    m_offset = 1.0 - variation;
    m_brightnessVariation = variation / (euler - 1 / euler);
}

uint8_t Breathe::getVariation() const
{
    return 100 - (m_offset * 100);
}

void Breathe::setCycleTime(uint16_t cycleTime)
{
    m_cycleTime = cycleTime;
}

} // namespace Mode
} // namespace LedWall
