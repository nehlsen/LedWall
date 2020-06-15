#include "ModeText.h"
#include <cJSON.h>
#include "utilities.h"
#include <QtCore/QDebug>

namespace LedWall {
namespace Mode {

bool ModeText::update()
{
    // FIXME scroll speed seems to be broken
    uint16_t delay = m_scrollSpeed == 0 ? 0 : 1000 / m_scrollSpeed;
    int64_t currentTime = esp_timer_get_time() / 1000;
    if ((m_scrollSpeed == 0 || currentTime - m_lastUpdate < delay) && m_lastUpdate > 0) {
        return false;
    }
    m_lastUpdate = currentTime;

//    m_matrix.fade(140);
    m_matrix.clear();

    m_displayText.setBackgroundColor(CRGB::Black).setColor(CRGB::Green);

    m_displayText.setY((m_matrix.getHeight() - m_displayText.getSize().height) / 2);
    m_displayText.setCanvas(
            0,
            0,
            max(m_displayText.getSize().width, m_matrix.getWidth()),
            m_matrix.getHeight(),
            GfxPrimitive::CanvasWrapAround
            );

    switch (getScrollMode()) {
        case ScrollInfinite:
        default:
            m_displayText.transform(scrollInfinite(), 0);
            break;

        case ScrollBounce:
            m_displayText.transform(scrollBounce(), 0);
            break;
    }

    m_displayText.render(m_matrix);

    return true;
}

void ModeText::readOptions(cJSON *root)
{
    cJSON_AddStringToObject(root, "text", getText().c_str());
    cJSON_AddNumberToObject(root, "scrollSpeed", getScrollSpeed());
    cJSON_AddNumberToObject(root, "scrollDirection", getScrollDirection());
    cJSON_AddNumberToObject(root, "scrollMode", getScrollMode());
}

bool ModeText::writeOptions(cJSON *root)
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

    objectItem = cJSON_GetObjectItem(root, "scrollDirection");
    if (objectItem && cJSON_IsNumber(objectItem)) {
        setScrollDirection(static_cast<ScrollDirection>(objectItem->valueint));
        changed = true;
    }

    objectItem = cJSON_GetObjectItem(root, "scrollMode");
    if (objectItem && cJSON_IsNumber(objectItem)) {
        setScrollMode(static_cast<ScrollMode>(objectItem->valueint));
        changed = true;
    }

    return changed;
}

const std::string &ModeText::getText() const
{
    return m_displayText.getText();
}

void ModeText::setText(const std::string &text)
{
    m_displayText.setText(text);
}

uint8_t ModeText::getScrollSpeed() const
{
    return m_scrollSpeed;
}

void ModeText::setScrollSpeed(uint8_t scrollSpeed)
{
    m_scrollSpeed = scrollSpeed;
}

ModeText::ScrollDirection ModeText::getScrollDirection() const
{
    return m_scrollDirection;
}

void ModeText::setScrollDirection(ScrollDirection direction)
{
    m_scrollDirection = direction;
}

ModeText::ScrollMode ModeText::getScrollMode() const
{
    return m_scrollMode;
}

void ModeText::setScrollMode(ScrollMode mode)
{
    m_scrollMode = mode;
}

int ModeText::scrollInfinite()
{
    int xTransform = m_currentStep * (getScrollDirection() == ScrollLeft ? -1 : 1);
    m_currentStep++;
    if (m_currentStep >= m_displayText.getCanvas().width) {
        m_currentStep = 0;
    }

    return xTransform;
}

int ModeText::scrollBounce()
{
    int xTransform = 0;

    if (m_displayText.getSize().width == m_matrix.getWidth()) {
        qDebug() << "no bounce!";
        return xTransform;
    }

    const bool isOuterBounce = m_displayText.getSize().width > m_matrix.getWidth();
    const int16_t maxScrollDistance = abs(m_matrix.getWidth() - m_displayText.getSize().width);

    if (m_currentStep == 2*maxScrollDistance) {
        m_currentStep = 0;
    }

    if (m_currentStep < maxScrollDistance) {
        // forward
        xTransform = m_currentStep;
    } else {
        // reverse
        xTransform = maxScrollDistance - (m_currentStep - maxScrollDistance);
    }

//    if outer bounce, direction is reversed
    xTransform *= (isOuterBounce ? -1 : 1);

    m_currentStep++;

    return xTransform;
}

} // namespace Mode
} // namespace LedWall
