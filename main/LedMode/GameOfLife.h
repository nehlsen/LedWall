#ifndef LEDWALL_GAMEOFLIFE_H
#define LEDWALL_GAMEOFLIFE_H

#include "LedMode.h"

namespace LedWall {
namespace Mode {

class GameOfLife : public LedMode
{
public:
    explicit GameOfLife(LedMatrix& matrix);
//    ~GameOfLife() override;

    bool update() override;

//    void readOptions(cJSON *root) override;
//    bool writeOptions(cJSON *root) override;

    uint16_t getGenerationDelay() const;

protected:
    uint8_t m_currentWorld = 0;
    CRGB* m_worlds[2];

    void clearWorld(int which);
    void drawWorld(int which);

    uint8_t width() const;
    uint8_t height() const;
    uint16_t xyToIndex(int8_t x, int8_t y) const;

    int64_t m_lastGenerationTime = 0;
    uint32_t m_generation = 0;

    // number of current generation
    uint32_t getGeneration() const;

    // advance to net generation, return generation number
    uint32_t advanceGeneration();

    // set generation to 0 and clear the world
    void reset();

    void beginIteration();
    void endIteration();

    bool isAliveAndShouldSurvive(uint8_t x, uint8_t y) const;
    bool isDeadAndShouldLive(uint8_t x, uint8_t y) const;

    // whether a live cell should survive
    bool shouldSurvive(uint8_t x, uint8_t y) const;
    // whether a dead cell should live
    bool shouldLive(uint8_t x, uint8_t y) const;

    CRGB averageNeighbourhoodColor(int8_t x, int8_t y) const;
    uint8_t countLiveNeighbours(int8_t x, int8_t y) const;

//    void setAlive(int8_t x, int8_t y);
    void setAlive(int8_t x, int8_t y, const CRGB &color);
    void setDead(int8_t x, int8_t y);

    bool isAlive(int8_t x, int8_t y) const;
    bool isDead(int8_t x, int8_t y) const;

    CRGB getColorDead() const;

    void randomSeed(uint8_t count);
};

} // namespace Mode
} // namespace LedWall

#endif //LEDWALL_GAMEOFLIFE_H
