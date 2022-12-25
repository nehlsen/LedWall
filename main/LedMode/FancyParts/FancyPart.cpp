#include "FancyPart.h"

#include <utility>

namespace LedWall::Mode::FancyParts {

FancyPart::FancyPart(LedMatrix &matrix, PartOptions options, std::string arguments):
    m_matrix(matrix), m_options(options), m_arguments(std::move(arguments))
{}



//FancyPart::FancyPart(uint16_t firstFrame, LedMatrix &matrix, PartOptions options):
//    m_matrix(matrix), m_options(options), m_firstFrame(firstFrame)
//{}
//
//FancyPart::FancyPart(FancyPart *previousPart, PartOptions options):
//    m_matrix(previousPart->m_matrix), m_options(options), m_firstFrame(previousPart->getLastFrame() + 1)
//{}

//uint16_t FancyPart::getFirstFrame() const
//{
//    return m_firstFrame;
//}
//
//uint16_t FancyPart::getLastFrame() const
//{
//    return getFirstFrame() + getFrameCount();
//}

void FancyPart::render(uint16_t relativeFrame)
{
    renderImpl(isForward() ? relativeFrame : getFrameCount() - relativeFrame);
}

bool FancyPart::hasOption(PartOptions option) const
{
    return m_options & option;
}

bool FancyPart::isForward() const
{
    return !hasOption(REVERSE);
}

bool FancyPart::isHorizontal() const
{
    return !hasOption(VERTICAL);
}

}
