#include "ExplodingLetters.h"
#include <Canvas.h>
#include "../utilities.h"

namespace LedWall::Mode::FancyParts {

const uint16_t targetZoom = 750;

ExplodingLetters::ExplodingLetters(LedMatrix &matrix, PartOptions options, const std::string& arguments):
    FancyPart(matrix, options, arguments), m_letters(arguments, CRGB::White)
{
}

uint16_t ExplodingLetters::getFrameCount() const
{
    return 25; // 25 frames = one second
}

void ExplodingLetters::renderImpl(uint16_t relativeFrame)
{
    if (relativeFrame == 0) {
        m_matrix.clear(false);
    }

    Canvas c(m_letters.pixels());

    if (relativeFrame >= m_stillFrames) {
        c.setSubPixelTransformationFactor(2)
        .setZoom(map(relativeFrame, m_stillFrames, getFrameCount(), 100, targetZoom))
        .applyTransformation();
    }

    c.renderCentered(m_matrix);
}

uint8_t ExplodingLetters::getStillFrames() const
{
    return m_stillFrames;
}

ExplodingLetters* ExplodingLetters::setStillFrames(uint8_t stillFrames)
{
    m_stillFrames = stillFrames;
    return this;
}

}
