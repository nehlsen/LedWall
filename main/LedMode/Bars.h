#ifndef LEDWALL_BARS_H
#define LEDWALL_BARS_H

#include "LedMode.h"

namespace LedWall {
namespace Mode {

class Bars : public LedMode
{
public:
    using LedMode::LedMode;

    bool update() override;

    void readOptions(cJSON *root) override;

    bool writeOptions(cJSON *root) override;

protected:
    uint8_t m_fadeRate = 200; // how fast to fade: 1-fast, 256-slow
    uint8_t m_barsRate = 60; // how many bars? 1-lots, 255-few

    void draw();
    void advanceDrawMode();
    enum {
        DrawVertical,
        DrawHorizontal,
        DrawDiagonalBl, // diagonal bottom left
        DrawDiagonalBr  // diagonal bottom right
    } m_drawMode = DrawVertical;
    uint8_t m_lastBarAt = 0;
    int64_t m_lastBarTime = 0;

    void drawVerticalBar(uint8_t x);
    void drawHorizontalBar(uint8_t y);
    void drawDiagonalBarBl(uint8_t frame);
    void drawDiagonalBarBr(uint8_t frame);
};

} // namespace Mode
} // namespace LedWall

#endif //LEDWALL_BARS_H
