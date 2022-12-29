#include "FlashingLetters.h"
#include "PartArguments.h"
#include <Canvas.h>

#define FRAMES_PER_LETTER 25
#define FRAMES_BLANK (FRAMES_PER_LETTER/2)

namespace LedWall::Mode::FancyParts {

uint16_t FlashingLetters::getFrameCount() const
{
    return getLetters().size() *  (getFramesPerLetter() + getBlankFramesPerLetter());
}

void FlashingLetters::renderImpl(uint16_t relativeFrame)
{
    auto partFrames = getFramesPerLetter() + getBlankFramesPerLetter();
    auto letterFrame = relativeFrame % partFrames;

    if (letterFrame < getFramesPerLetter()) {
        int letterIndex = relativeFrame / partFrames;

        auto letter = getLetters().substr(letterIndex, 1);
        m_text.setText(letter);

        Canvas(m_text.pixels()).renderCentered(m_matrix);
    }
}

std::string FlashingLetters::getLetters() const
{
    return m_arguments.argument(0, "fallback");
}

int FlashingLetters::getFramesPerLetter() const
{
    return m_arguments.argument(1, FRAMES_PER_LETTER);
}

int FlashingLetters::getBlankFramesPerLetter() const
{
    return m_arguments.argument(2, FRAMES_BLANK);
}

}
