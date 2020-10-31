#include "FancyDemoPart.h"

namespace LedWall::Mode {

FancyDemoPart::FancyDemoPart(uint16_t firstFrame, LedMatrix &matrix):
    m_matrix(matrix), m_firstFrame(firstFrame)
{}

FancyDemoPart::FancyDemoPart(FancyDemoPart *previousPart):
    m_matrix(previousPart->m_matrix), m_firstFrame(previousPart->getLastFrame() + 1)
{}

uint16_t FancyDemoPart::getFirstFrame() const
{
    return m_firstFrame;
}

uint16_t FancyDemoPart::getLastFrame() const
{
    return getFirstFrame() + getFrameCount();
}

}
