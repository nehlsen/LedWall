#include "MultiBars.h"
#include "utilities.h"
#include <cJSON.h>

// it is not possible to have less than this bars
#define ABSOLUTE_MIN_BARS 1
// it is not possible to have more than this bars
#define ABSOLUTE_MAX_BARS 10

MultiBars::MultiBars()
{
    initBars();
}

void MultiBars::update()
{
    for (int i = 0; i < FastLED.size(); i++) {
        FastLED.leds()[i].nscale8(m_fadeRate);
    }
    uint16_t delay = map(0xff - m_barTravelSpeed, 0, 255, 0, 1000);
    int64_t currentTime = esp_timer_get_time() / 1000;
    if (currentTime - m_lastFrame > delay || currentTime < 1) {
        for (int i = 0; i < m_numberOfBars; ++i) {
            m_bars[i]->drawFrame();
            
            if (!m_bars[i]->canDrawFrame()) {
                Bar *const newBar = createRandomBar();
                delete m_bars[i];
                m_bars[i] = newBar;
            }
        }
        m_lastFrame = currentTime;
    }
}

void MultiBars::readOptions(cJSON *root)
{
    cJSON_AddNumberToObject(root, "fadeRate", m_fadeRate);
    cJSON_AddNumberToObject(root, "barTravelSpeed", m_barTravelSpeed);
    cJSON_AddNumberToObject(root, "numberOfBars", m_numberOfBars);
    cJSON_AddNumberToObject(root, "maximumFrameDelay", m_maximumFrameDelay);
    cJSON_AddBoolToObject(root, "barKeepsColor", m_barKeepsColor);
    cJSON_AddBoolToObject(root, "blendColor", m_blendColor);
}

bool MultiBars::writeOptions(cJSON *root)
{
    cJSON *const requestedFadeRate = cJSON_GetObjectItem(root, "fadeRate");
    if (requestedFadeRate) {
        int value = requestedFadeRate->valueint;
        m_fadeRate = constrain(value, 0, 255);
    }

    cJSON *const requestedTravelSpeed = cJSON_GetObjectItem(root, "barTravelSpeed");
    if (requestedTravelSpeed) {
        int value = requestedTravelSpeed->valueint;
        m_barTravelSpeed = constrain(value, 0, 255);
    }

    cJSON *const requestedNumberOfBars = cJSON_GetObjectItem(root, "numberOfBars");
    if (requestedNumberOfBars) {
        int value = requestedNumberOfBars->valueint;
        setNumberOfBars(constrain(value, ABSOLUTE_MIN_BARS, ABSOLUTE_MAX_BARS));
    }

    cJSON *const requestedMaximumFrameDelay = cJSON_GetObjectItem(root, "maximumFrameDelay");
    if (requestedMaximumFrameDelay) {
        int value = requestedMaximumFrameDelay->valueint;
        m_maximumFrameDelay = constrain(value, 0, 255);
    }

    cJSON *const requestedKeepColor = cJSON_GetObjectItem(root, "barKeepsColor");
    if (requestedKeepColor) {
        m_barKeepsColor = cJSON_IsBool(requestedKeepColor) ? cJSON_IsTrue(requestedKeepColor) : true;
    }

    cJSON *const requestedBlendColor = cJSON_GetObjectItem(root, "blendColor");
    if (requestedBlendColor) {
        m_blendColor = cJSON_IsBool(requestedBlendColor) ? cJSON_IsTrue(requestedBlendColor) : true;
    }

    return requestedFadeRate || requestedTravelSpeed || requestedNumberOfBars || requestedMaximumFrameDelay || requestedKeepColor || requestedBlendColor;
}

void MultiBars::initBars()
{
    m_bars.resize(m_numberOfBars);

    for (int i = 0; i < m_numberOfBars; ++i) {
        m_bars[i] = new Bar(randomDrawMode(), randomDrawDirection(), m_barKeepsColor, m_blendColor);
    }
}

void MultiBars::setNumberOfBars(uint8_t count)
{
    if (count > m_numberOfBars) {
        // add (count - m_numberOfBars) bars
        for (int i = 0; i < count - m_numberOfBars; ++i) {
            m_bars.push_back(createRandomBar());
        }
    }

    if (count < m_numberOfBars) {
        // delete (m_numberOfBars - count) bars
        for (int i = 0; i < m_numberOfBars - count; ++i) {
            delete m_bars.back();
            m_bars.pop_back();
        }
    }

    m_numberOfBars = count;
}

MultiBars::Bar* MultiBars::createRandomBar()
{
    uint8_t breakCounter = 0;

    while (true) {
        auto mode = randomDrawMode();
        auto direction = randomDrawDirection();

        for (auto &bar : m_bars) {
            if (bar->mode != mode || bar->direction != direction) {
                return new Bar(mode, direction, m_barKeepsColor, m_blendColor, random8(m_maximumFrameDelay));
            }
        }

        if (++breakCounter > 15) {
            return new Bar(Bar::DrawVertical, Bar::DirectionForward, m_barKeepsColor, m_blendColor);
        }
    }
}

MultiBars::Bar::DrawMode MultiBars::randomDrawMode() const
{
    return (Bar::DrawMode)random8(Bar::DrawModeCount);
}

MultiBars::Bar::DrawDirection MultiBars::randomDrawDirection() const
{
    return (Bar::DrawDirection)random8(Bar::DrawDirectionCount);
}

/* ******************************  ******************************  ******************************  ****************************** */

MultiBars::Bar::Bar(MultiBars::Bar::DrawMode drawMode, MultiBars::Bar::DrawDirection drawDirection, bool constColor, bool blend, uint8_t emptyFrames):
    mode(drawMode), direction(drawDirection), constantColor(constColor), blendColor(blend),
    currentFrame(-1 * emptyFrames), hue(random8())
{
//    ESP_LOGI("Bar", "New Bar mode:%d, direction: %d, constColor: %d, blend: %d, emptyFrames: %d", mode, direction, constantColor, blendColor, emptyFrames);
}

bool MultiBars::Bar::canDrawFrame() const
{
    if (currentFrame < 0) {
        return true;
    }

    switch (mode) {
        case DrawVertical:
            return currentFrame < matrixWidth;
        case DrawHorizontal:
            return currentFrame < matrixHeight;

        case DrawDiagonalBl:
        case DrawDiagonalBr:
            return currentFrame < matrixWidth + matrixHeight;

        default:
        case DrawModeCount:
            break;
    }

    return false;
}

void MultiBars::Bar::drawFrame()
{
    if (currentFrame < 0) {
        ++currentFrame;
        return;
    }

    if (!canDrawFrame()) {
        return;
    }

    switch (mode) {
        case DrawVertical:
            drawVerticalBar(direction == DirectionForward ? currentFrame : matrixWidth - currentFrame - 1);
            break;
        case DrawHorizontal:
            drawHorizontalBar(direction == DirectionForward ? currentFrame : matrixHeight - currentFrame - 1);
            break;
        case DrawDiagonalBl:
            drawDiagonalBarBl(direction == DirectionForward ? currentFrame : matrixWidth + matrixHeight - currentFrame - 1);
            break;
        case DrawDiagonalBr:
            drawDiagonalBarBr(direction == DirectionForward ? currentFrame : matrixWidth + matrixHeight - currentFrame - 1);
            break;

        default:
        case DrawModeCount:
            break;
    }

    ++currentFrame;
}

void MultiBars::Bar::drawVerticalBar(uint8_t x)
{
    if (direction == DirectionForward) {
        for (uint8_t y = 0; y < matrixHeight; ++y) {
            draw(x, y);
        }
    } else {
        // signed to be able to reach -1 for abort condition
        for (int8_t y = matrixHeight-1; y >= 0; --y) {
            draw(x, y);
        }
    }
}

void MultiBars::Bar::drawHorizontalBar(uint8_t y)
{
    if (direction == DirectionForward) {
        for (uint8_t x = 0; x < matrixWidth; ++x) {
            draw(x, y);
        }
    } else {
        // signed to be able to reach -1 for abort condition
        for (int8_t x = matrixWidth-1; x >= 0; --x) {
            draw(x, y);
        }
    }
}

void MultiBars::Bar::drawDiagonalBarBl(uint8_t frame)
{
    if (direction == DirectionForward) {
        for (uint8_t y = 0; y < matrixHeight; ++y) {
            draw(frame - y, y);
        }
    } else {
        // signed to be able to reach -1 for abort condition
        for (int8_t y = matrixHeight-1; y >= 0; --y) {
            draw(frame - y, y);
        }
    }
}

void MultiBars::Bar::drawDiagonalBarBr(uint8_t frame)
{
    if (direction == DirectionForward) {
        for (uint8_t y = 0; y < matrixHeight; ++y) {
            draw(matrixWidth - frame - 1 + y, y);
        }
    } else {
        // signed to be able to reach -1 for abort condition
        for (int8_t y = matrixHeight-1; y >= 0; --y) {
            draw(matrixWidth - frame - 1 + y, y);
        }
    }
}

void MultiBars::Bar::draw(uint8_t x, uint8_t y)
{
    auto color = CHSV(constantColor ? hue : random8(), 255, 255);

    if (blendColor) {
        FastLED.leds()[xyToIndex(x, y)] += color;
    } else {
        FastLED.leds()[xyToIndex(x, y)] = color;
    }
}
