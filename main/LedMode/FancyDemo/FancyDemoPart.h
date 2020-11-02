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
    explicit FancyDemoPart(uint16_t firstFrame, LedMatrix &matrix, PartOptions options = NO_OPS);
    explicit FancyDemoPart(FancyDemoPart *previousPart, PartOptions options = NO_OPS);

    uint16_t getFirstFrame() const;
    uint16_t getLastFrame() const;

    virtual void render(uint16_t relativeFrame);

protected:
    virtual void renderImpl(uint16_t relativeFrame) = 0;

    LedMatrix &m_matrix;
    virtual uint16_t getFrameCount() const = 0;

    PartOptions m_options;
    bool hasOption(PartOptions option) const;
    bool isForward() const;
    bool isHorizontal() const;

private:
    const uint16_t m_firstFrame;
};

}

#endif //LEDWALL_FANCYDEMOPART_H
