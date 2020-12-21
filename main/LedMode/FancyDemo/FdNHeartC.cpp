#include "FdNHeartC.h"
#include <Text.h>

namespace LedWall::Mode {

void FdNHeartC::renderImpl(uint16_t relativeFrame)
{
    Text letters("N   C", CRGB::White);
    letters.setY(4);

    Pixels heartPixels {
            {2, 8, CRGB::Red}, {3, 8, CRGB::Red}, {7, 8, CRGB::Red}, {8, 8, CRGB::Red},
            {1, 7, CRGB::Red}, {4, 7, CRGB::Red}, {6, 7, CRGB::Red}, {9, 7, CRGB::Red},
            {0, 6, CRGB::Red}, {5, 6, CRGB::Red}, {10, 6, CRGB::Red},
            {0, 5, CRGB::Red}, {10, 5, CRGB::Red},
            {1, 4, CRGB::Red}, {9, 4, CRGB::Red},
            {2, 3, CRGB::Red}, {8, 3, CRGB::Red},
            {3, 2, CRGB::Red}, {7, 2, CRGB::Red},
            {4, 1, CRGB::Red}, {6, 1, CRGB::Red},
            {5, 0, CRGB::Red}, {5, 0, CRGB::Red},
    };

    GfxPrimitive heart;
    heart.merge(heartPixels);
    heart.transform(7, 3);

    letters.merge(heart);
    letters.render(m_matrix);
}

uint16_t FdNHeartC::getFrameCount() const
{
    return 28;
}

}
