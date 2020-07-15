#include "GameOfLife.h"

// TODO make it configurable?
#define SCALEDOWN_FACTOR 240

namespace LedWall {
namespace Mode {

GameOfLife::GameOfLife(LedMatrix &matrix) : LedMode(matrix)
{
    m_worlds[0] = static_cast<CRGB*>(malloc(width() * height() * sizeof(CRGB)));
    clearWorld(0);
    m_worlds[1] = static_cast<CRGB*>(malloc(width() * height() * sizeof(CRGB)));
    clearWorld(1);

    randomSeed(80);
}

bool GameOfLife::update()
{
    int64_t currentTime = esp_timer_get_time() / 1000;

    // draw current generation at 2 fps and advance
    if (currentTime - m_lastGenerationTime > getGenerationDelay()) {
//        ESP_LOGI("GameOfLife", "Current Generation: %d", getGeneration());
        drawWorld(m_currentWorld);

        advanceGeneration();
        m_lastGenerationTime = currentTime;
    }

    // fade whole world at 25 fps
    m_matrix.fade(SCALEDOWN_FACTOR);

    return true;
}

uint16_t GameOfLife::getGenerationDelay() const
{
    // TODO make it configurable
    return 250;
}

void GameOfLife::clearWorld(int which)
{
    for (int n = 0; n < width() * height(); ++n) {
        m_worlds[which][n] = getColorDead();
    }
}

void GameOfLife::drawWorld(int which)
{
    for (uint8_t x = 0; x < width(); ++x) {
        for (uint8_t y = 0; y < height(); ++y) {
            // FIXME do not draw black
//            if (m_worlds[which][xyToIndex(x,y)] == CRGB(0, 0, 0)) {
//                continue;
//            }

            m_matrix.pixel(x, y) = m_worlds[which][xyToIndex(x,y)];
        }
    }
}

uint8_t GameOfLife::width() const
{
    return m_matrix.getWidth();
}

uint8_t GameOfLife::height() const
{
    return m_matrix.getHeight();
}

uint16_t GameOfLife::xyToIndex(int8_t x, int8_t y) const
{
    while (x < 0) {
        x += width();
    }
    while (x >= width()) {
        x -= width();
    }

    while (y < 0) {
        y += height();
    }
    while (y >= height()) {
        y -= height();
    }

    return x + y * width();
}

uint32_t GameOfLife::getGeneration() const
{
    return m_generation;
}

uint32_t GameOfLife::advanceGeneration()
{
    beginIteration();
    for (uint8_t x = 0; x < width(); ++x) {
        for (uint8_t y = 0; y < height(); ++y) {
            if (isAliveAndShouldSurvive(x, y)) {
                setAlive(x, y, m_worlds[m_currentWorld][xyToIndex(x, y)]);
            }
            if (isDeadAndShouldLive(x, y)) {
                setAlive(x, y, averageNeighbourhoodColor(x, y));
            }
        }
    }
    endIteration();

    m_generation++;
    return m_generation;
}

void GameOfLife::reset()
{
    clearWorld(0);
    clearWorld(1);
    m_lastGenerationTime = 0;
    m_generation = 0;
}

void GameOfLife::beginIteration()
{
    clearWorld(m_currentWorld == 0 ? 1 : 0);
}

void GameOfLife::endIteration()
{
    m_currentWorld = m_currentWorld == 0 ? 1 : 0;
}

bool GameOfLife::isAliveAndShouldSurvive(uint8_t x, uint8_t y) const
{
    return isAlive(x, y) && shouldSurvive(x, y);
}

bool GameOfLife::isDeadAndShouldLive(uint8_t x, uint8_t y) const
{
    return isDead(x, y) && shouldLive(x, y);
}

bool GameOfLife::shouldSurvive(uint8_t x, uint8_t y) const
{
    const uint8_t liveNeighbours = countLiveNeighbours(x, y);
//    if (countLiveNeighbours>0) ESP_LOGI("GameOfLife", "shouldSurvive %d/%d, neighbours:%d", x, y, countLiveNeighbours);
    return liveNeighbours == 2 || liveNeighbours == 3;
}

bool GameOfLife::shouldLive(uint8_t x, uint8_t y) const
{
    const uint8_t liveNeighbours = countLiveNeighbours(x, y);
//    if (countLiveNeighbours>0) ESP_LOGI("GameOfLife", "shouldLive %d/%d, neighbours:%d", x, y, countLiveNeighbours);
    return liveNeighbours == 3;
}

CRGB GameOfLife::averageNeighbourhoodColor(int8_t x, int8_t y) const
{
    uint16_t sum_r = 0, sum_g = 0, sum_b = 0;
    uint8_t liveNeighbours = 0;

    std::vector<std::pair<uint8_t, uint8_t>> neighbours;
    neighbours.emplace_back(x-1, y-1);
    neighbours.emplace_back(x, y-1);
    neighbours.emplace_back(x+1, y-1);

    neighbours.emplace_back(x-1, y);
    neighbours.emplace_back(x+1, y);

    neighbours.emplace_back(x-1, y+1);
    neighbours.emplace_back(x, y+1);
    neighbours.emplace_back(x+1, y+1);

    for (auto &neighbour : neighbours) {
        const CRGB &neigh = m_worlds[m_currentWorld][xyToIndex(neighbour.first, neighbour.second)];
        sum_r += neigh.r;
        sum_g += neigh.g;
        sum_b += neigh.b;
        
        if (neigh.r != 0 || neigh.g != 0 || neigh.b != 0) {
            liveNeighbours += 1;
        }
    }

    const uint8_t avg_r = sum_r/liveNeighbours;
    const uint8_t avg_g = sum_g/liveNeighbours;
    const uint8_t avg_b = sum_b/liveNeighbours;

    return {avg_r, avg_g, avg_b};
}

uint8_t GameOfLife::countLiveNeighbours(int8_t x, int8_t y) const
{
    uint8_t alive = 0;

    alive += isAlive(x-1, y-1) ? 1 : 0;
    alive += isAlive(x, y-1) ? 1 : 0;
    alive += isAlive(x+1, y-1) ? 1 : 0;

    alive += isAlive(x-1, y) ? 1 : 0;
    alive += isAlive(x+1, y) ? 1 : 0;

    alive += isAlive(x-1, y+1) ? 1 : 0;
    alive += isAlive(x, y+1) ? 1 : 0;
    alive += isAlive(x+1, y+1) ? 1 : 0;

    return alive;
}

//void GameOfLife::setAlive(int8_t x, int8_t y)
//{
//    setAlive(x, y, averageNeighbourhoodColor(x, y));
//}

void GameOfLife::setAlive(int8_t x, int8_t y, const CRGB &color)
{
    m_worlds[m_currentWorld == 0 ? 1 : 0][xyToIndex(x, y)] = color;
}

void GameOfLife::setDead(int8_t x, int8_t y)
{
    m_worlds[m_currentWorld == 0 ? 1 : 0][xyToIndex(x, y)] = getColorDead();
}

bool GameOfLife::isAlive(int8_t x, int8_t y) const
{
    return !isDead(x, y);
}

bool GameOfLife::isDead(int8_t x, int8_t y) const
{
    // FIXME because we use average colors and fading we should define some threshold for being dead... not only BLACK
    return m_worlds[m_currentWorld][xyToIndex(x, y)] == getColorDead();
}

CRGB GameOfLife::getColorDead() const
{
    return {0, 0, 0};
}

void GameOfLife::randomSeed(uint8_t count)
{
    beginIteration();
    for (uint16_t cell = count; cell > 0; --cell) {
        uint8_t random_x = random8(width());
        uint8_t random_y = random8(height());
        CRGB randomColor(random8(), random8(), random8());

        if (!isAlive(random_x, random_y)) {
            setAlive(random_x, random_y, randomColor);
        } else {
            // FIXME find another cell
        }
    }
    endIteration();
}


} // namespace Mode
} // namespace LedWall
