#include "GameOfLife.h"
#include <PixelMap.h>

// TODO make it configurable?
#define SCALEDOWN_FACTOR 240

namespace LedWall {
namespace Mode {

class World
{
public:
    World(uint8_t width, uint8_t height): m_width(width), m_height(height) {}

    virtual CRGB get(Point p) const = 0;
    virtual void set(Point p, const CRGB &color) = 0;
    virtual void clear() = 0;
    virtual void clear(Point p) = 0;
    virtual bool isSet(Point p) const = 0;

protected:
    uint8_t m_width;
    uint8_t m_height;

    Point fixPoint(Point p) const
    {
        int16_t x = p.x;
        int16_t y = p.y;

        while (x < 0) {
            x += m_width;
        }
        while (x >= m_width) {
            x -= m_width;
        }

        while (y < 0) {
            y += m_height;
        }
        while (y >= m_height) {
            y -= m_height;
        }

        return {x, y};
    }
};

class WorldCrgbArray : public World
{
public:
    WorldCrgbArray(uint8_t width, uint8_t height): World(width, height)
    {
        m_world = static_cast<CRGB*>(malloc(m_width * m_height * sizeof(CRGB)));
    }

    CRGB get(Point p) const override
    {
        return m_world[pointToIndex(p)];
    }

    void set(Point p, const CRGB &color) override
    {
        m_world[pointToIndex(p)] = color;
    }

    void clear() override
    {
        for (int n = 0; n < m_width * m_height; ++n) {
            m_world[n].setRGB(0, 0, 0);
        }
    }

    void clear(Point p) override
    {
        set(p, {0, 0, 0});
    }

    bool isSet(Point p) const override
    {
        const CRGB &c(get(p));
        return c.r != 0 || c.g != 0 || c.b != 0;
    }

protected:
    CRGB* m_world;

    uint16_t pointToIndex(Point p) const
    {
        p = fixPoint(p);

        return p.x + p.y * m_width;
    }
};

class WorldPixelMap : public World
{
protected:
    PixelMap m_world;
};

/**********************************************************************************************************************/

GameOfLife::GameOfLife(LedMatrix &matrix) : LedMode(matrix)
{
    m_worlds[0] = new WorldCrgbArray(width(), height());
    m_worlds[0]->clear();
    m_worlds[1] = new WorldCrgbArray(width(), height());
    m_worlds[1]->clear();

    randomSeed(80);
}

bool GameOfLife::update()
{
    const int64_t currentTime = esp_timer_get_time() / 1000;
    // draw current generation at 2 fps and advance
    if (currentTime - m_lastGenerationTime > getGenerationDelay() || currentTime < 1) {
        advanceGeneration();
        m_lastGenerationTime = currentTime;
    }

    m_matrix.fade(SCALEDOWN_FACTOR);
    drawWorld(m_currentWorld);

    return true;
}

uint16_t GameOfLife::getGenerationDelay() const
{
    // TODO make it configurable
    return 500;
}

void GameOfLife::clearWorld(int which)
{
    m_worlds[which]->clear();
}

void GameOfLife::drawWorld(int which)
{
    for (uint8_t x = 0; x < width(); ++x) {
        for (uint8_t y = 0; y < height(); ++y) {
            if (m_worlds[which]->isSet({x, y})) {
                m_matrix.pixel(x, y) = m_worlds[which]->get({x, y});
            }
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
                setAlive(x, y, m_worlds[m_currentWorld]->get({x, y}));
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
        if (!m_worlds[m_currentWorld]->isSet({neighbour.first, neighbour.second})) {
            continue;
        }

        const CRGB &neigh = m_worlds[m_currentWorld]->get({neighbour.first, neighbour.second});
        sum_r += neigh.r;
        sum_g += neigh.g;
        sum_b += neigh.b;
        
        liveNeighbours += 1;
    }

    if (liveNeighbours == 0) {
        return {0, 0, 0};
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

void GameOfLife::setAlive(int8_t x, int8_t y, const CRGB &color)
{
    m_worlds[m_currentWorld == 0 ? 1 : 0]->set({x, y}, color);
}

void GameOfLife::setDead(int8_t x, int8_t y)
{
    m_worlds[m_currentWorld == 0 ? 1 : 0]->clear({x, y});
}

bool GameOfLife::isAlive(int8_t x, int8_t y) const
{
    return m_worlds[m_currentWorld]->isSet({x, y});
}

bool GameOfLife::isDead(int8_t x, int8_t y) const
{
    // FIXME because we use average colors and fading we should define some threshold for being dead... not only BLACK
    return !isAlive(x, y);
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
