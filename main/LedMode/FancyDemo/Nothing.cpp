#include "Nothing.h"

namespace LedWall::Mode {

void Nothing::renderImpl(uint16_t relativeFrame)
{}

uint16_t Nothing::getFrameCount() const
{
    return 0;
}

}
