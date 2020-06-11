#ifndef LEDWALL_MULTI_BARS_H
#define LEDWALL_MULTI_BARS_H

#include <vector>
#include "LedMode.h"
#include <Line.h>

namespace LedWall {
namespace Mode {

class MultiBars: public LedMode
{
public:
    explicit MultiBars(LedMatrix& matrix);

    bool update() override;

    void readOptions(cJSON *root) override;
    bool writeOptions(cJSON *root) override;

    const uint8_t &getFadeRate() const;
    void setFadeRate(const uint8_t &fadeRate);

    const uint8_t &getBarTravelSpeed() const;
    void setBarTravelSpeed(const uint8_t &barTravelSpeed);

    uint8_t getNumberOfBars() const;
    void setNumberOfBars(const uint8_t &numberOfBars);

    const uint8_t &getMaximumFrameDelay() const;
    void setMaximumFrameDelay(const uint8_t &maximumFrameDelay);

    bool isBarKeepsColor() const;
    void setBarKeepsColor(bool barKeepsColor);

    bool isBlendColor() const;
    void setBlendColor(bool blendColor);

protected:
    uint8_t m_fadeRate = 200; // how fast to fade: 1-fast, 256-slow
    uint8_t m_barTravelSpeed = 245; // how fast bars travel? 1-slow, 255-fast
//    uint8_t m_numberOfBars = 3; // absolute number of parallel bars
    uint8_t m_maximumFrameDelay = 7; // maximum frames to delay a new bar? 0-no pause, 5-up to 5 empty frames, ...
    bool m_barKeepsColor = true; // whether bars keep their color while moving or random color each frame
    bool m_blendColor = true; // whether each pixel blends or replaces current color

    class Bar {
    public:
        enum DrawMode {
            DrawVertical = 0,
            DrawHorizontal,
            DrawDiagonalBl, // diagonal bottom left
            DrawDiagonalBr, // diagonal bottom right
            DrawSectorScanTl, // sector scanner - origin: top left
            DrawSectorScanTr,
            DrawSectorScanBl,
            DrawSectorScanBr,

            DrawModeCount
        };

        enum DrawDirection {
            DirectionForward = 0,
            DirectionReverse,

            DrawDirectionCount
        };

        Bar(LedMatrix& matrix, DrawMode drawMode, DrawDirection drawDirection, bool constantColor, bool blendColor, uint8_t emptyFrames = 0);

        LedMatrix& matrix;
        const DrawMode mode;
        const DrawDirection direction;
        const bool constantColor;
        const bool blendColor;

        int8_t currentFrame;
        uint8_t hue;

        bool canDrawFrame() const;
        void drawFrame();

        void drawVerticalBar(uint8_t x);
        void drawHorizontalBar(uint8_t y);
        void drawDiagonalBarBl(uint8_t frame);
        void drawDiagonalBarBr(uint8_t frame);
        void draw(uint8_t x, uint8_t y);
        void drawSectorScanTl(uint8_t frame);
        void drawSectorScanTr(uint8_t frame);
        void drawSectorScanBl(uint8_t frame);
        void drawSectorScanBr(uint8_t frame);
        void drawLine(Point p0, Point p1);
    };

    std::vector<Bar*> m_bars;
    void initBars(int count = 3);

    // semi random - already running config will be prevented
    Bar *createRandomBar();
    Bar::DrawMode randomDrawMode() const;
    Bar::DrawDirection randomDrawDirection() const;

    // timestamp of last frame rendered
    int64_t m_lastFrame = 0;
};

} // namespace Mode
} // namespace LedWall

#endif //LEDWALL_MULTI_BARS_H
