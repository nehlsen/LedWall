#include "Text.h"
#include <Text.h>
#include <cJSON.h>

namespace LedWall {
namespace Mode {

void Text::update()
{
    uint16_t delay = 1000 / m_scrollSpeed;
    int64_t currentTime = esp_timer_get_time() / 1000;
    if (currentTime - m_lastUpdate < delay && m_lastUpdate > 0) {
        return;
    }
    m_lastUpdate = currentTime;

//    m_matrix.fade(140);
    m_matrix.clear();

    ::Text displayText;
    displayText.setBackgroundColor(CRGB::Black).setColor(CRGB::Green);
    displayText.setText(m_text + "  ");

    const int yOffset = (m_matrix.getHeight() - displayText.getSize().height) / 2;
    displayText.setY(yOffset);
    displayText.setCanvas(
            0,
            0,
            displayText.getSize().width,
            m_matrix.getHeight(),
            GfxPrimitive::CanvasWrapAround
            );
    displayText.transform(-m_currentStep++, 0);
    displayText.render(m_matrix);

    if (m_currentStep == displayText.getSize().width) {
        m_currentStep = 0;
    }
}

void Text::readOptions(cJSON *root)
{
    cJSON_AddStringToObject(root, "text", m_text.c_str());
    cJSON_AddNumberToObject(root, "scrollSpeed", m_scrollSpeed);
}

bool Text::writeOptions(cJSON *root)
{
    bool changed = false;

    cJSON *objectItem = cJSON_GetObjectItem(root, "text");
    if (objectItem && cJSON_IsString(objectItem) && strlen(objectItem->valuestring) <= 255) {
        setText(objectItem->valuestring);
        changed = true;
    }

    objectItem = cJSON_GetObjectItem(root, "scrollSpeed");
    if (objectItem && cJSON_IsNumber(objectItem)) {
        setScrollSpeed(constrain(objectItem->valueint, 1, 60));
        changed = true;
    }

    return changed;
}

const std::string &Text::getText() const
{
    return m_text;
}

void Text::setText(const std::string &text)
{
    m_text = text;
}

uint8_t Text::getScrollSpeed() const
{
    return m_scrollSpeed;
}

void Text::setScrollSpeed(uint8_t scrollSpeed)
{
    m_scrollSpeed = scrollSpeed;
}

} // namespace Mode
} // namespace LedWall
