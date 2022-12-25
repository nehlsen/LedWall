#ifndef LEDWALL_FANCYSCRIPT_FANCYPART_H
#define LEDWALL_FANCYSCRIPT_FANCYPART_H

#include <cstdint>
#include <string>

class LedMatrix;

namespace LedWall::Mode::FancyParts {

enum PartOptions {
    FORWARD    = 0b0000,
    REVERSE    = 0b0001,
    HORIZONTAL = 0b0000,
    VERTICAL   = 0b0010,

    NO_OPS     = 0b0000,
};

class FancyPart
{
public:
    FancyPart(LedMatrix &matrix, PartOptions options, std::string arguments);
    virtual ~FancyPart() = default;

    virtual uint16_t getFrameCount() const = 0;

    virtual void render(uint16_t relativeFrame);

protected:
    virtual void renderImpl(uint16_t relativeFrame) = 0;

    LedMatrix &m_matrix;

    const PartOptions m_options;
    bool hasOption(PartOptions option) const;
    bool isForward() const;
    bool isHorizontal() const;

    const std::string m_arguments;
};

}

#endif //LEDWALL_FANCYSCRIPT_FANCYPART_H
