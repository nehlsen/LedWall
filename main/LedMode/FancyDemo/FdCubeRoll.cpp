#include "FdCubeRoll.h"
#include <LedMatrix.h>
#include <Canvas.h>
#include <Rect.h>

namespace LedWall::Mode {

// target roll? 360 = one rotation, 180 = one half rotation
static const uint16_t degreesToRoll = 180;

// degrees / per frame = frames
// 360 / 8 = 45
// 360 / 9 = 40
// 360 / 10 = 36
// 360 / 12 = 30
// 360 / 15 = 24
static const uint8_t degreesPerFrame = 15;

void FdCubeRoll::renderImpl(uint16_t relativeFrame)
{
    uint8_t cubeSize = 9;

    uint8_t x = 0;
    uint8_t y = 0;

    uint8_t w = cubeSize;
    uint8_t h = cubeSize;

    Canvas c(Rect(x, y, w, h, CRGB::White).pixels());
    c
        .setRotation((relativeFrame+1) * degreesPerFrame, c.getCenter())
        .applyTransformation()
        .render(m_matrix, {int16_t((m_matrix.getWidth() - cubeSize) / 2), int16_t((m_matrix.getHeight() - cubeSize) / 2)})
    ;
}

uint16_t FdCubeRoll::getFrameCount() const
{
    return degreesToRoll / degreesPerFrame;
}

}
