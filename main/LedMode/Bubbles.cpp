#include "Bubbles.h"
#include "utilities.h"
#include <Circle.h>
#include <cJSON.h>
#include <iostream>

namespace LedWall::Mode {

// it is not possible to have less than this Bubbles
#define ABSOLUTE_MIN_BUBBLES 1
// it is not possible to have more than this Bubbles
#define ABSOLUTE_MAX_BUBBLES 10

Bubbles::Bubbles(LedMatrix &matrix):
    LedMode(matrix)
{
    setMaximumBubbleSize(std::min(m_matrix.getWidth(), m_matrix.getHeight()) * 1/2);
    initBubbles(7);
}

void Bubbles::readOptions(cJSON *root)
{
    cJSON_AddNumberToObject(root, "numberOfBubbles", getNumberOfBubbles());
    cJSON_AddNumberToObject(root, "maximumBubbleSize", getMaximumBubbleSize());
    cJSON_AddNumberToObject(root, "speed", getSpeed());
    cJSON_AddNumberToObject(root, "maximumFrameDelay", getMaximumFrameDelay());
}

bool Bubbles::writeOptions(cJSON *root)
{
    cJSON *const requestedNumberOfBubbles = cJSON_GetObjectItem(root, "numberOfBubbles");
    if (requestedNumberOfBubbles) {
        setNumberOfBubbles(constrain(requestedNumberOfBubbles->valueint, ABSOLUTE_MIN_BUBBLES, ABSOLUTE_MAX_BUBBLES));
    }

    cJSON *const requestedMaximumBubbleSize = cJSON_GetObjectItem(root, "maximumBubbleSize");
    if (requestedMaximumBubbleSize) {
        setMaximumBubbleSize(constrain(requestedMaximumBubbleSize->valueint, 0, 255));
    }

    cJSON *const requestedSpeed = cJSON_GetObjectItem(root, "speed");
    if (requestedSpeed) {
        setSpeed(constrain(requestedSpeed->valueint, 0, 255));
    }

    cJSON *const requestedMaximumFrameDelay = cJSON_GetObjectItem(root, "maximumFrameDelay");
    if (requestedMaximumFrameDelay) {
        setMaximumFrameDelay(constrain(requestedMaximumFrameDelay->valueint, 0, 255));
    }

    return requestedNumberOfBubbles || requestedMaximumBubbleSize || requestedMaximumFrameDelay;
}

bool Bubbles::update()
{
    uint16_t delay = map(0xff - m_speed, 0, 255, 0, 500);
    int64_t currentTime = esp_timer_get_time() / 1000;
    const bool drawLastFrameAgain = (currentTime - m_lastFrame) < delay && currentTime > 0;

    m_matrix.clear(); // FIXME
    for (int i = 0; i < getNumberOfBubbles(); ++i) {
        m_bubbles[i]->drawFrame(drawLastFrameAgain);

        if (!m_bubbles[i]->canDrawFrame()) {
            auto *const newBubble = createBubble();
            delete m_bubbles[i];
            m_bubbles[i] = newBubble;
        }
    }

    if (!drawLastFrameAgain) m_lastFrame = currentTime; // both in 1.000th of a second

    return true;
}

uint8_t Bubbles::getNumberOfBubbles() const
{
    return m_bubbles.size();
}

void Bubbles::setNumberOfBubbles(const uint8_t &numberOfBubbles)
{
    if (numberOfBubbles > m_bubbles.size()) {
        const unsigned int countBubblesToAdd = numberOfBubbles - m_bubbles.size();
        for (int i = 0; i < countBubblesToAdd; ++i) {
            m_bubbles.push_back(createBubble());
        }
    } else if (numberOfBubbles < m_bubbles.size()) {
        const unsigned int countBubblesToRemove = m_bubbles.size() - numberOfBubbles;
        for (int i = 0; i < countBubblesToRemove; ++i) {
            delete m_bubbles.back();
            m_bubbles.pop_back();
        }
    }
}

uint8_t Bubbles::getMaximumBubbleSize() const
{
    return m_maximumBubbleSize;
}

void Bubbles::setMaximumBubbleSize(uint8_t maximumBubbleSize)
{
    m_maximumBubbleSize = maximumBubbleSize;
}

uint8_t Bubbles::getSpeed() const
{
    std::cout << "getSpeed " << (int)m_speed << std::endl;
    return m_speed;
}

void Bubbles::setSpeed(uint8_t speed)
{
    m_speed = speed;
}

const uint8_t & Bubbles::getMaximumFrameDelay() const
{
    return m_maximumFrameDelay;
}

void Bubbles::setMaximumFrameDelay(const uint8_t &maximumFrameDelay)
{
    m_maximumFrameDelay = maximumFrameDelay;
}

void Bubbles::initBubbles(int count)
{
    m_bubbles.resize(count);

    for (int i = 0; i < count; ++i) {
        m_bubbles[i] = createBubble();
    }
}

Bubbles::Bubble* Bubbles::createBubble()
{
    // FIXME should this be configurable?
    const uint8_t maxFadeFrameCount = 50;

    return new Bubble(
        m_matrix,
        m_maximumBubbleSize,
        random8(maxFadeFrameCount),
        random8(m_maximumFrameDelay)
        );
}

/* ****************************************************************************************************************** */

Bubbles::Bubble::Bubble(LedMatrix &pMatrix, uint8_t pMaxSize, uint8_t pFadeFrameCount, uint8_t emptyFrames):
    matrix(pMatrix),
    currentFrame(-1 * emptyFrames),
    maxSize(0), fadeFrameCount(pFadeFrameCount),
    x(random8(pMatrix.getWidth())), y(random8(pMatrix.getHeight())),
    hue(random8())
{
    maxSize = pMaxSize < 1 ? 0 : random8(pMaxSize);
}

bool Bubbles::Bubble::canDrawFrame() const
{
    if (currentFrame < 0) {
        return true;
    }

    if (currentFrame < (maxSize + fadeFrameCount)) {
        return true;
    }

    return false;
}

void Bubbles::Bubble::drawFrame(bool drawLastFrameAgain)
{
    if (currentFrame < 0) {
        if (!drawLastFrameAgain) {
            ++currentFrame;
        }
        return;
    }

    if (!canDrawFrame()) {
        return;
    }

    uint8_t radius = std::min(currentFrame, (int8_t)maxSize);
    int colorValue = 255;
    auto renderMode = GfxPrimitive::RenderModeOverwrite;

    if (currentFrame >= maxSize) {
        colorValue = map(
                currentFrame - maxSize,
                0,
                fadeFrameCount,
                255,
                0
                );
        renderMode = GfxPrimitive::RenderModeAdd;
    }

    const CHSV &color = CHSV(hue, 255, colorValue);
    ::Circle c(x, y, radius);
    c.setBorderColor(color).setFillColor(color);
    c.render(matrix, {0, 0}, renderMode);

    if (!drawLastFrameAgain) {
        ++currentFrame;
    }
}

}
