#ifndef LEDWALL_MULTI_BARS_H
#define LEDWALL_MULTI_BARS_H

#include "LedMode.h"

#define NUMBER_OF_SIMULTANEOUS_BARS 2

class MultiBars: public LedMode
{
public:
    MultiBars();

    void update() override;

//    void readOptions(cJSON *root) override;

//    bool writeOptions(cJSON *root) override;

protected:
    uint8_t m_fadeRate = 200; // how fast to fade: 1-fast, 256-slow
    uint8_t m_barsRate = 60; // how many bars? 1-lots, 255-few

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

        Bar(DrawMode drawMode, DrawDirection drawDirection);

        const DrawMode mode;
        const DrawDirection direction;
        uint8_t currentFrame;

        bool canDrawFrame() const;
        void drawFrame();

        void drawVerticalBar(uint8_t x);
        void drawHorizontalBar(uint8_t y);
        void drawDiagonalBarBl(uint8_t frame);
        void drawDiagonalBarBr(uint8_t frame);
    };

    Bar* m_bars[NUMBER_OF_SIMULTANEOUS_BARS];
    void initBars();

    // semi random - already running config will be prevented
    Bar *createRandomBar();
    Bar::DrawMode randomDrawMode() const;
    Bar::DrawDirection randomDrawDirection() const;

    // timestamp of last frame rendered
    int64_t m_lastFrame = 0;
};

#endif //LEDWALL_MULTI_BARS_H
