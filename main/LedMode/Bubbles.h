#ifndef LEDWALL_BUBBLES_H
#define LEDWALL_BUBBLES_H

#include "LedMode.h"
#include <vector>

namespace LedWall::Mode {

class Bubbles : public LedMode
{
public:
    explicit Bubbles(LedMatrix& matrix);

    bool update() override;

    void readOptions(cJSON *root) override;
    bool writeOptions(cJSON *root) override;

    uint8_t getNumberOfBubbles() const;
    void setNumberOfBubbles(const uint8_t &numberOfBubbles);

    uint8_t getMaximumBubbleSize() const;
    void setMaximumBubbleSize(uint8_t maximumBubbleSize);

    uint8_t getSpeed() const;
    void setSpeed(uint8_t speed);

    const uint8_t &getMaximumFrameDelay() const;
    void setMaximumFrameDelay(const uint8_t &maximumFrameDelay);

protected:
    uint8_t m_maximumBubbleSize = 5;
    uint8_t m_speed = 255;
    uint8_t m_maximumFrameDelay = 27; // maximum frames to delay a new bubble. 0-no pause, 5-up to 5 empty frames, ...

    class Bubble {
    public:
        explicit Bubble(LedMatrix& matrix, uint8_t maxSize, uint8_t fadeFrameCount, uint8_t emptyFrames = 0);

        LedMatrix& matrix;

        int8_t currentFrame;
        uint8_t maxSize;
        uint8_t fadeFrameCount;
        uint8_t x, y;
        uint8_t hue;

        bool canDrawFrame() const;
        void drawFrame(bool drawLastFrameAgain);
    };

    std::vector<Bubble*> m_bubbles;
    void initBubbles(int count = 3);
    Bubble* createBubble();

    // timestamp of last frame rendered
    int64_t m_lastFrame = 0;
};

} // namespace LedWall

#endif // LEDWALL_BUBBLES_H
