#include "FancyDemoPart.h"

namespace LedWall::Mode {

FancyDemoPart::FancyDemoPart(uint16_t firstFrame, LedMatrix &matrix, PartOptions options):
    m_matrix(matrix), m_firstFrame(firstFrame), m_options(options)
{}

FancyDemoPart::FancyDemoPart(FancyDemoPart *previousPart, PartOptions options):
    m_matrix(previousPart->m_matrix), m_firstFrame(previousPart->getLastFrame() + 1), m_options(options)
{}

uint16_t FancyDemoPart::getFirstFrame() const
{
    return m_firstFrame;
}

uint16_t FancyDemoPart::getLastFrame() const
{
    return getFirstFrame() + getFrameCount();
}

void FancyDemoPart::render(uint16_t relativeFrame)
{
    renderImpl(isForward() ? relativeFrame : getFrameCount() - relativeFrame);
}

bool FancyDemoPart::hasOption(PartOptions option) const
{
    return m_options & option;
}

bool FancyDemoPart::isForward() const
{
    return !hasOption(REVERSE);
}

bool FancyDemoPart::isHorizontal() const
{
    return !hasOption(VERTICAL);
}

}
