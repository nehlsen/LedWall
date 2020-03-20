#include "MultiBars.h"
#include "utilities.h"

// delay new bars
// make bars have a static color

MultiBars::MultiBars()
{
    initBars();
}

void MultiBars::update()
{
    for (int i = 0; i < FastLED.size(); i++) {
        FastLED.leds()[i].nscale8(m_fadeRate);
    }

    uint16_t delay = map(m_barsRate, 0, 255, 0, 1000);
    int64_t currentTime = esp_timer_get_time() / 1000;
    if (currentTime - m_lastFrame > delay || currentTime < 1) {
        for (int i = 0; i < NUMBER_OF_SIMULTANEOUS_BARS; ++i) {
            m_bars[i]->drawFrame();
            
            if (!m_bars[i]->canDrawFrame()) {
                Bar *const newBar = createRandomBar();
                delete m_bars[i];
                m_bars[i] = newBar;
            }
        }
        m_lastFrame = currentTime;
    }

    vTaskDelay(20 / portTICK_PERIOD_MS);
}

void MultiBars::initBars()
{
    for (int i = 0; i < NUMBER_OF_SIMULTANEOUS_BARS; ++i) {
        m_bars[i] = new Bar(randomDrawMode(), randomDrawDirection());
    }
}

MultiBars::Bar* MultiBars::createRandomBar()
{
    uint8_t breakCounter = 0;

    while (true) {
        auto mode = randomDrawMode();
        auto direction = randomDrawDirection();

        for (auto &bar : m_bars) {
            if (bar->mode != mode || bar->direction != direction) {
                return new Bar(mode, direction);
            }
        }

        if (++breakCounter > 15) {
            return new Bar(Bar::DrawVertical, Bar::DirectionForward);
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

MultiBars::Bar::Bar(MultiBars::Bar::DrawMode drawMode, MultiBars::Bar::DrawDirection drawDirection):
    mode(drawMode), direction(drawDirection), currentFrame(0)
{
//    ESP_LOGI("Bar", "New Bar mode:%d, direction: %d", mode, direction);
}

bool MultiBars::Bar::canDrawFrame() const
{
    switch (mode) {
        case DrawVertical:
            return currentFrame < CONFIG_NUM_LEDS_HORIZONTAL;
        case DrawHorizontal:
            return currentFrame < CONFIG_NUM_LEDS_VERTICAL;

        case DrawDiagonalBl:
        case DrawDiagonalBr:
            return currentFrame < CONFIG_NUM_LEDS_HORIZONTAL + CONFIG_NUM_LEDS_VERTICAL - 1;

        default:
        case DrawModeCount:
            break;
    }

    return false;
}

void MultiBars::Bar::drawFrame()
{
    if (!canDrawFrame()) {
        return;
    }

    switch (mode) {
        case DrawVertical:
            drawVerticalBar(direction == DirectionForward ? currentFrame : CONFIG_NUM_LEDS_HORIZONTAL - currentFrame - 1);
            break;
        case DrawHorizontal:
            drawHorizontalBar(direction == DirectionForward ? currentFrame : CONFIG_NUM_LEDS_VERTICAL - currentFrame - 1);
            break;
        case DrawDiagonalBl:
            drawDiagonalBarBl(direction == DirectionForward ? currentFrame : CONFIG_NUM_LEDS_HORIZONTAL - currentFrame - 1);
            break;
        case DrawDiagonalBr:
            drawDiagonalBarBr(direction == DirectionForward ? currentFrame : CONFIG_NUM_LEDS_HORIZONTAL - currentFrame - 1);
            break;

        default:
        case DrawModeCount:
            break;
    }

    ++currentFrame;
}

void MultiBars::Bar::drawVerticalBar(uint8_t x)
{
    uint8_t randomHue = random8();

    auto draw = [x, randomHue](uint8_t y) {
        FastLED.leds()[xyToIndex(x, y)].setHSV(randomHue, 255, 255);
    };

    if (direction == DirectionForward) {
        for (uint8_t y = 0; y < CONFIG_NUM_LEDS_VERTICAL; ++y) {
            draw(y);
        }
    } else {
        // signed to be able to reach -1 for abort condition
        for (int8_t y = CONFIG_NUM_LEDS_VERTICAL-1; y >= 0; --y) {
            draw(y);
        }
    }
}

void MultiBars::Bar::drawHorizontalBar(uint8_t y)
{
    uint8_t randomHue = random8();

    auto draw = [y, randomHue](uint8_t x) {
        FastLED.leds()[xyToIndex(x, y)].setHSV(randomHue, 255, 255);
    };

    if (direction == DirectionForward) {
        for (uint8_t x = 0; x < CONFIG_NUM_LEDS_HORIZONTAL; ++x) {
            draw(x);
        }
    } else {
        // signed to be able to reach -1 for abort condition
        for (int8_t x = CONFIG_NUM_LEDS_HORIZONTAL-1; x >= 0; --x) {
            draw(x);
        }
    }
}

void MultiBars::Bar::drawDiagonalBarBl(uint8_t frame)
{
    uint8_t randomHue = random8();

    auto draw = [frame, randomHue](uint8_t y) {
        FastLED.leds()[xyToIndex(frame - y, y)].setHSV(randomHue, 255, 255);
    };

    if (direction == DirectionForward) {
        for (uint8_t y = 0; y < CONFIG_NUM_LEDS_VERTICAL; ++y) {
            draw(y);
        }
    } else {
        // signed to be able to reach -1 for abort condition
        for (int8_t y = CONFIG_NUM_LEDS_VERTICAL-1; y >= 0; --y) {
            draw(y);
        }
    }
}

void MultiBars::Bar::drawDiagonalBarBr(uint8_t frame)
{
    uint8_t randomHue = random8();

    auto draw = [frame, randomHue](uint8_t y) {
        FastLED.leds()[xyToIndex(CONFIG_NUM_LEDS_HORIZONTAL - frame - 1 + y, y)].setHSV(randomHue, 255, 255);
    };

    if (direction == DirectionForward) {
        for (uint8_t y = 0; y < CONFIG_NUM_LEDS_VERTICAL; ++y) {
            draw(y);
        }
    } else {
        // signed to be able to reach -1 for abort condition
        for (int8_t y = CONFIG_NUM_LEDS_VERTICAL-1; y >= 0; --y) {
            draw(y);
        }
    }
}
