#include "FdRollingCube.h"
#include <LedMatrix.h>
#include <Canvas.h>
#include <Rect.h>

namespace LedWall::Mode {

// 360 / 8 = 45
// 360 / 9 = 40
// 360 / 10 = 36
// 360 / 12 = 30
// 360 / 15 = 24
static const uint8_t degreesPerFrame = 15;

FdRollingCube::FdRollingCube(FancyDemoPart *previousPart, bool directionForward):
    FancyDemoPart(previousPart), m_directionForward(directionForward)
{
}

void FdRollingCube::renderImpl(uint16_t relativeFrame)
{
//    if (frame < 0 || frame >= frameCount) return frameCount;

    uint8_t cubeSize = 9;

    uint8_t x = 0;
    uint8_t y = 0;

    uint8_t w = cubeSize;
    uint8_t h = cubeSize;

    Canvas c(Rect(x, y, w, h, CRGB::White).pixels());
    c.setRotation((relativeFrame+1) * degreesPerFrame * (m_directionForward ? 1 : -1), c.getCenter())
            .applyTransformation()
//     .render(m_matrix)
            .render(m_matrix, {int16_t((m_matrix.getWidth() - cubeSize) / 2), int16_t((m_matrix.getHeight() - cubeSize) / 2)})
//     .renderCentered(m_matrix)
            ;
}

uint16_t FdRollingCube::getFrameCount() const
{
    return 360 / degreesPerFrame;
}

}
