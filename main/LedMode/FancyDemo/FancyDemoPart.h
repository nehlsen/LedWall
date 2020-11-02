#ifndef LEDWALL_FANCYDEMOPART_H
#define LEDWALL_FANCYDEMOPART_H

#include <cstdint>

class LedMatrix;

namespace LedWall::Mode {

enum PartOptions {
    FORWARD    = 0b0000,
    REVERSE    = 0b0001,
    HORIZONTAL = 0b0000,
    VERTICAL   = 0b0010,

    NO_OPS     = 0b0000,
};

class FancyDemoPart
{
public:
    explicit FancyDemoPart(uint16_t firstFrame, LedMatrix &matrix);
    explicit FancyDemoPart(FancyDemoPart *previousPart);

    uint16_t getFirstFrame() const; // absolut frame index
    uint16_t getLastFrame() const; // absolut frame index

//    uint16_t getFrameCount() const;
//    uint16_t getRemainingFrames() const;

    virtual void render(uint16_t relativeFrame) = 0;

protected:
    LedMatrix &m_matrix;

//    void setFrameCount(uint16_t frameCount);

    virtual uint16_t getFrameCount() const = 0;

private:
    const uint16_t m_firstFrame;
};

}

#endif //LEDWALL_FANCYDEMOPART_H
