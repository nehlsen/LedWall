#include "ModeText.h"
#include <Text.h>
#include <cJSON.h>
#include "utilities.h"
#include <QtCore/QDebug>

namespace LedWall {
namespace Mode {

bool ModeText::update()
{
    uint16_t delay = m_scrollSpeed == 0 ? 0 : 1000 / m_scrollSpeed;
    int64_t currentTime = esp_timer_get_time() / 1000;
    if ((m_scrollSpeed == 0 || currentTime - m_lastUpdate < delay) && m_lastUpdate > 0) {
        return false;
    }
    m_lastUpdate = currentTime;

//    m_matrix.fade(140);
    m_matrix.clear();

    ::Text displayText;
    displayText.setBackgroundColor(CRGB::Black).setColor(CRGB::Green);
    displayText.setText(m_text);

    displayText.setY((m_matrix.getHeight() - displayText.getSize().height) / 2);
    displayText.setCanvas(
            0,
            0,
            max(displayText.getSize().width, m_matrix.getWidth()),
            m_matrix.getHeight(),
            GfxPrimitive::CanvasWrapAround
            );

    int xTransform = 0;
    if (getScrollMode() == ScrollBounce) {
//        qDebug() << "ScrollBounce";
        if (displayText.getSize().width < m_matrix.getWidth()) {
            // text is smaller than matrix and bounces inside
            qDebug() << "text < matrix, step" << m_currentStep;

//            const int16_t freeSpaceWidth = displayText.getCanvas().width - displayText.getSize().width; // has been working
            const int16_t freeSpaceWidth = m_matrix.getWidth() - displayText.getSize().width; // sounds more reasonable
            if (m_currentStep == 2*freeSpaceWidth) {
                m_currentStep = 0;
            }

            // forward
            xTransform = m_currentStep;

            // reverse
            if (m_currentStep >= freeSpaceWidth) {
                xTransform = freeSpaceWidth - (m_currentStep - freeSpaceWidth);
            }

            qDebug() << "xTransform" << xTransform;

            m_currentStep++;
        } else if (displayText.getSize().width > m_matrix.getWidth()) {
            // text is larger than matrix ...
            qDebug() << "text > matrix, step" << m_currentStep;

            const int16_t missingSpaceWidth = displayText.getSize().width - m_matrix.getWidth();
            if (m_currentStep == 2*missingSpaceWidth) {
                qDebug() << m_currentStep << "==" << 2*missingSpaceWidth << " --- RESET current step to 0";
                m_currentStep = 0;
            }

            // forward
            xTransform = m_currentStep;

            // reverse
            if (m_currentStep >= missingSpaceWidth) {
                xTransform = missingSpaceWidth - (m_currentStep - missingSpaceWidth);
            }

            xTransform *= -1;
            qDebug() << "xTransform" << xTransform << "(" << missingSpaceWidth << ")";

            m_currentStep++;
        } else {
            qDebug() << "text = matrix";
            // text and matrix are same size. scroll?!
        }
//        m_currentStep == displayText.getCanvas().width - m_matrix.getWidth()
    } else {
        qDebug() << "ScrollInfinite";
        xTransform = m_currentStep * (getScrollDirection() == ScrollLeft ? -1 : 1);
        m_currentStep++;
        if (m_currentStep >= displayText.getCanvas().width) {
            m_currentStep = 0;
        }
    }

    displayText.transform(xTransform, 0);
    displayText.render(m_matrix);

    return true;
}

void ModeText::readOptions(cJSON *root)
{
    cJSON_AddStringToObject(root, "text", m_text.c_str());
    cJSON_AddNumberToObject(root, "scrollSpeed", m_scrollSpeed);
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
    return m_text;
}

void ModeText::setText(const std::string &text)
{
    m_text = text;
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

} // namespace Mode
} // namespace LedWall
