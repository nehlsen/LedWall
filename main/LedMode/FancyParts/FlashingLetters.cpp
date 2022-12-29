#include "FlashingLetters.h"
#include "PartArguments.h"
#include <Canvas.h>
#include <Text.h>

#define FRAMES_COUNT_EXPLOSION 5

namespace LedWall::Mode::FancyParts {

uint16_t FlashingLetters::getFrameCount() const
{
    return getLetters().size() *  (getFramesPerLetter() + getBlankFramesPerLetter());
}

void FlashingLetters::renderImpl(uint16_t relativeFrame)
{
    auto partFrames = getFramesPerLetter() + getBlankFramesPerLetter();
    auto letterFrame = relativeFrame % partFrames;

    if (letterFrame < (getFramesPerLetter() - 1)) {
        int letterIndex = relativeFrame / partFrames;

        auto letter = getLetters().substr(letterIndex, 1);
        Canvas(Text().setText(letter).pixels()).renderCentered(m_matrix);
    }
}

std::string FlashingLetters::getLetters() const
{
    return m_arguments.argument(0, "flashing!");
}

int FlashingLetters::getFramesPerLetter() const
{
    return m_arguments.argument(1, FRAMES_COUNT_EXPLOSION);
}

int FlashingLetters::getBlankFramesPerLetter() const
{
    return m_arguments.argument(2, (getFramesPerLetter() / 2));
}

}
