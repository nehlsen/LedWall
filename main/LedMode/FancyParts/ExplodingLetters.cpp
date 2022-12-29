#include "ExplodingLetters.h"
#include <Canvas.h>
#include "../utilities.h"

#define FRAMES_COUNT_EXPLOSION 25

namespace LedWall::Mode::FancyParts {

const uint16_t targetZoom = 750;

ExplodingLetters::ExplodingLetters(LedMatrix &matrix, PartOptions options, const std::string& arguments):
    FancyPart(matrix, options, arguments)
{
}

uint16_t ExplodingLetters::getFrameCount() const
{
    return getFrameCountExplosion() + getStillFrameCount();
}

void ExplodingLetters::renderImpl(uint16_t relativeFrame)
{
    Canvas c(Text().setText(getLetters()).pixels());

    if (relativeFrame >= getStillFrameCount()) {
        c.setSubPixelTransformationFactor(2)
        .setZoom(map(relativeFrame, getStillFrameCount(), getFrameCount(), 100, targetZoom))
        .applyTransformation();
    }

    c.renderCentered(m_matrix);
}

std::string ExplodingLetters::getLetters() const
{
    return m_arguments.argument(0, "exploding!");
}

int ExplodingLetters::getFrameCountExplosion() const
{
    return m_arguments.argument(1, FRAMES_COUNT_EXPLOSION);
}

int ExplodingLetters::getStillFrameCount() const
{
    return m_arguments.argument(2, 5);
}

}
