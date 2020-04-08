#ifndef LEDWALL_MULTI_BARS_H
#define LEDWALL_MULTI_BARS_H

#include <vector>
#include "LedMode.h"

class MultiBars: public LedMode
{
public:
    MultiBars();

    void update() override;

    void readOptions(cJSON *root) override;
    bool writeOptions(cJSON *root) override;

protected:
    uint8_t m_fadeRate = 200; // how fast to fade: 1-fast, 256-slow
    uint8_t m_barTravelSpeed = 245; // how fast bars travel? 1-slow, 255-fast
    uint8_t m_numberOfBars = 3; // absolute number of parallel bars
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

            DrawModeCount
        };

        enum DrawDirection {
            DirectionForward = 0,
            DirectionReverse,

            DrawDirectionCount
        };

        Bar(DrawMode drawMode, DrawDirection drawDirection, bool constantColor, bool blendColor, uint8_t emptyFrames = 0);

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
    };

    std::vector<Bar*> m_bars;
    void initBars();
    void setNumberOfBars(uint8_t count);

    // semi random - already running config will be prevented
    Bar *createRandomBar();
    Bar::DrawMode randomDrawMode() const;
    Bar::DrawDirection randomDrawDirection() const;

    // timestamp of last frame rendered
    int64_t m_lastFrame = 0;
};

#endif //LEDWALL_MULTI_BARS_H
