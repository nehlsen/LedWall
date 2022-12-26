#include "Sprinkle.h"
#include <LedMatrix.h>
#include <GfxPrimitive.h>

namespace LedWall::Mode::FancyParts {

uint16_t Sprinkle::getFrameCount() const
{
    return 2 * 25; // 2 seconds * 25 fpd
}

void Sprinkle::renderImpl(uint16_t relativeFrame)
{
    GfxPrimitive prim;

    const int count = (m_matrix.getWidth() * m_matrix.getHeight()) * 0.15;
    for (int n = 0; n < count; ++n) {
        int x = random8(m_matrix.getWidth());
        int y = random8(m_matrix.getHeight());
        prim.merge(Pixel(x, y, CRGB::White));
    }

    prim.render(m_matrix);
}

}

