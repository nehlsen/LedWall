#include "Nothing.h"

#include "LedMatrix.h"

namespace LedWall::Mode::FancyParts {

uint16_t Nothing::getFrameCount() const
{
    return 1;
}

void Nothing::renderImpl(uint16_t relativeFrame)
{
    m_matrix.clear(false);
}

}
