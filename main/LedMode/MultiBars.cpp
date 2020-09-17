#include "MultiBars.h"
#include "utilities.h"
#include <cJSON.h>

namespace LedWall {
namespace Mode {

// it is not possible to have less than this bars
#define ABSOLUTE_MIN_BARS 1
// it is not possible to have more than this bars
#define ABSOLUTE_MAX_BARS 10

MultiBars::MultiBars(LedMatrix& matrix):
    LedMode(matrix)
{
    initBars();
}

bool MultiBars::update()
{
    m_matrix.fade(m_fadeRate);

    uint16_t delay = map(0xff - m_barTravelSpeed, 0, 255, 0, 1000);
    int64_t currentTime = esp_timer_get_time() / 1000;
    if (currentTime - m_lastFrame > delay || currentTime < 1) {
        for (int i = 0; i < getNumberOfBars(); ++i) {
            // FIXME frame should be drawn in all calls to update
            //       drawFrame should accept argument: (bool)incrementFrameCount
            //       delay should control this boolean and not the draw itself
            m_bars[i]->drawFrame();
            
            if (!m_bars[i]->canDrawFrame()) {
                Bar *const newBar = createRandomBar();
                delete m_bars[i];
                m_bars[i] = newBar;
            }
        }
        m_lastFrame = currentTime;
    }

    return true;
}

void MultiBars::readOptions(cJSON *root)
{
    cJSON_AddNumberToObject(root, "fadeRate", getFadeRate());
    cJSON_AddNumberToObject(root, "barTravelSpeed", getBarTravelSpeed());
    cJSON_AddNumberToObject(root, "numberOfBars", getNumberOfBars());
    cJSON_AddNumberToObject(root, "maximumFrameDelay", getMaximumFrameDelay());
}

bool MultiBars::writeOptions(cJSON *root)
{
    cJSON *const requestedFadeRate = cJSON_GetObjectItem(root, "fadeRate");
    if (requestedFadeRate) {
        setFadeRate(constrain(requestedFadeRate->valueint, 0, 255));
    }

    cJSON *const requestedTravelSpeed = cJSON_GetObjectItem(root, "barTravelSpeed");
    if (requestedTravelSpeed) {
        setBarTravelSpeed(constrain(requestedTravelSpeed->valueint, 0, 255));
    }

    cJSON *const requestedNumberOfBars = cJSON_GetObjectItem(root, "numberOfBars");
    if (requestedNumberOfBars) {
        setNumberOfBars(constrain(requestedNumberOfBars->valueint, ABSOLUTE_MIN_BARS, ABSOLUTE_MAX_BARS));
    }

    cJSON *const requestedMaximumFrameDelay = cJSON_GetObjectItem(root, "maximumFrameDelay");
    if (requestedMaximumFrameDelay) {
        setMaximumFrameDelay(constrain(requestedMaximumFrameDelay->valueint, 0, 255));
    }

    return requestedFadeRate || requestedTravelSpeed || requestedNumberOfBars || requestedMaximumFrameDelay;
}

const uint8_t &MultiBars::getFadeRate() const
{
    return m_fadeRate;
}

void MultiBars::setFadeRate(const uint8_t &fadeRate)
{
    m_fadeRate = fadeRate;
}

const uint8_t &MultiBars::getBarTravelSpeed() const
{
    return m_barTravelSpeed;
}

void MultiBars::setBarTravelSpeed(const uint8_t &barTravelSpeed)
{
    m_barTravelSpeed = barTravelSpeed;
}

uint8_t MultiBars::getNumberOfBars() const
{
    return m_bars.size();
}

void MultiBars::setNumberOfBars(const uint8_t &numberOfBars)
{
    if (numberOfBars > m_bars.size()) {
        const unsigned int countBarsToAdd = numberOfBars - m_bars.size();
        for (int i = 0; i < countBarsToAdd; ++i) {
            m_bars.push_back(createRandomBar());
        }
    } else if (numberOfBars < m_bars.size()) {
        const unsigned int countBarsToRemove = m_bars.size() - numberOfBars;
        for (int i = 0; i < countBarsToRemove; ++i) {
            delete m_bars.back();
            m_bars.pop_back();
        }
    }
}

const uint8_t &MultiBars::getMaximumFrameDelay() const
{
    return m_maximumFrameDelay;
}

void MultiBars::setMaximumFrameDelay(const uint8_t &maximumFrameDelay)
{
    m_maximumFrameDelay = maximumFrameDelay;
}

void MultiBars::initBars(int count)
{
    m_bars.resize(count);

    for (int i = 0; i < count; ++i) {
        m_bars[i] = new Bar(m_matrix, randomDrawMode(), randomDrawDirection());
    }
}

MultiBars::Bar* MultiBars::createRandomBar()
{
    uint8_t breakCounter = 0;

    while (true) {
        auto mode = randomDrawMode();
        auto direction = randomDrawDirection();

        for (auto &bar : m_bars) {
            if (bar->mode != mode && bar->direction != direction) {
                return new Bar(m_matrix, mode, direction, random8(m_maximumFrameDelay));
            }
        }

        if (++breakCounter > 15) {
            return new Bar(m_matrix, Bar::DrawVertical, Bar::DirectionForward);
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

MultiBars::Bar::Bar(LedMatrix& pMatrix, MultiBars::Bar::DrawMode drawMode, MultiBars::Bar::DrawDirection drawDirection, uint8_t emptyFrames):
    matrix(pMatrix), mode(drawMode), direction(drawDirection),
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
            return currentFrame < matrix.getWidth();
        case DrawHorizontal:
            return currentFrame < matrix.getHeight();

        case DrawDiagonalBl:
        case DrawDiagonalBr:
            return currentFrame < matrix.getWidth() + matrix.getHeight();

        case DrawSectorScanTl:
        case DrawSectorScanTr:
        case DrawSectorScanBl:
        case DrawSectorScanBr:
            return currentFrame < matrix.getWidth() + matrix.getHeight();

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
            drawVerticalBar(direction == DirectionForward ? currentFrame : matrix.getWidth() - currentFrame - 1);
            break;
        case DrawHorizontal:
            drawHorizontalBar(direction == DirectionForward ? currentFrame : matrix.getHeight() - currentFrame - 1);
            break;

        case DrawDiagonalBl:
            drawDiagonalBarBl(direction == DirectionForward ? currentFrame : matrix.getWidth() + matrix.getHeight() - currentFrame - 1);
            break;
        case DrawDiagonalBr:
            drawDiagonalBarBr(direction == DirectionForward ? currentFrame : matrix.getWidth() + matrix.getHeight() - currentFrame - 1);
            break;

        case DrawSectorScanTl:
            drawSectorScanTl(direction == DirectionForward ? currentFrame : matrix.getWidth() + matrix.getHeight() - currentFrame - 1);
            break;
        case DrawSectorScanTr:
            drawSectorScanTr(direction == DirectionForward ? currentFrame : matrix.getWidth() + matrix.getHeight() - currentFrame - 1);
            break;
        case DrawSectorScanBl:
            drawSectorScanBl(direction == DirectionForward ? currentFrame : matrix.getWidth() + matrix.getHeight() - currentFrame - 1);
            break;
        case DrawSectorScanBr:
            drawSectorScanBr(direction == DirectionForward ? currentFrame : matrix.getWidth() + matrix.getHeight() - currentFrame - 1);
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
        for (uint8_t y = 0; y < matrix.getHeight(); ++y) {
            draw(x, y);
        }
    } else {
        // signed to be able to reach -1 for abort condition
        for (int8_t y = matrix.getHeight()-1; y >= 0; --y) {
            draw(x, y);
        }
    }
}

void MultiBars::Bar::drawHorizontalBar(uint8_t y)
{
    if (direction == DirectionForward) {
        for (uint8_t x = 0; x < matrix.getWidth(); ++x) {
            draw(x, y);
        }
    } else {
        // signed to be able to reach -1 for abort condition
        for (int8_t x = matrix.getWidth()-1; x >= 0; --x) {
            draw(x, y);
        }
    }
}

void MultiBars::Bar::drawDiagonalBarBl(uint8_t frame)
{
    if (direction == DirectionForward) {
        for (uint8_t y = 0; y < matrix.getHeight(); ++y) {
            draw(frame - y, y);
        }
    } else {
        // signed to be able to reach -1 for abort condition
        for (int8_t y = matrix.getHeight()-1; y >= 0; --y) {
            draw(frame - y, y);
        }
    }
}

void MultiBars::Bar::drawDiagonalBarBr(uint8_t frame)
{
    if (direction == DirectionForward) {
        for (uint8_t y = 0; y < matrix.getHeight(); ++y) {
            draw(matrix.getWidth() - frame - 1 + y, y);
        }
    } else {
        // signed to be able to reach -1 for abort condition
        for (int8_t y = matrix.getHeight()-1; y >= 0; --y) {
            draw(matrix.getWidth() - frame - 1 + y, y);
        }
    }
}

void MultiBars::Bar::draw(uint8_t x, uint8_t y)
{
    matrix.pixel(x, y) += CHSV(hue, 255, 255);
}

void MultiBars::Bar::drawSectorScanTl(uint8_t frame)
{
    const int16_t top = matrix.getHeight() - 1;
    const int16_t right = matrix.getWidth() - 1;
    const Point origin = {0, top};
    int16_t target_x = right;
    int16_t target_y = top - frame;
    if (target_y < 0) {
        target_x += target_y;
        target_x = target_x < 0 ? 0 : target_x;
        target_y = 0;
    }

    drawLine(origin, {target_x, target_y});
}

void MultiBars::Bar::drawSectorScanTr(uint8_t frame)
{
    const int16_t top = matrix.getHeight() - 1;
    const int16_t right = matrix.getWidth() - 1;
    const Point origin = {right, top};
    int16_t target_x = 0;
    int16_t target_y = top - frame;
    if (target_y < 0) {
        target_x += -target_y;
        target_x = target_x >= right ? right : target_x;
        target_y = 0;
    }

    drawLine(origin, {target_x, target_y});
}

void MultiBars::Bar::drawSectorScanBl(uint8_t frame)
{
    const int16_t top = matrix.getHeight() - 1;
    const int16_t right = matrix.getWidth() - 1;
    const Point origin = {0, 0};
    int16_t target_x = right;
    int16_t target_y = frame;
    if (target_y >= top) {
        target_x -= target_y-top;
        target_x = target_x < 0 ? 0 : target_x;
        target_y = top;
    }

    drawLine(origin, {target_x, target_y});
}

void MultiBars::Bar::drawSectorScanBr(uint8_t frame)
{
    const int16_t top = matrix.getHeight() - 1;
    const int16_t right = matrix.getWidth() - 1;
    const Point origin = {right, 0};
    int16_t target_x = 0;
    int16_t target_y = frame;
    if (target_y >= top) {
        target_x += target_y-top;
        target_x = target_x > right ? right : target_x;
        target_y = top;
    }

    drawLine(origin, {target_x, target_y});
}

void MultiBars::Bar::drawLine(Point p0, Point p1)
{
    ::Line l(p0, p1);
    l.setColor(CHSV(hue, 255, 255));
    l.render(matrix, {0, 0}, GfxPrimitive::RenderModeAdd);
}

} // namespace Mode
} // namespace LedWall
