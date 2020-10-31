#include "FancyDemoPart6.h"

namespace LedWall::Mode {

static const uint8_t degreesPerFrame = 10;

FancyDemoPart6::FancyDemoPart6(FancyDemoPart *previousPart, bool directionForward):
    FancyDemoPart(previousPart), m_directionForward(directionForward)
{
    initLines();
}

void FancyDemoPart6::render(uint16_t relativeFrame)
{
//    if (frame < 0 || frame >= frameCount) return frameCount;

    Canvas c(m_lines.pixels());

    c
        .setRotation((relativeFrame) * degreesPerFrame * (m_directionForward ? 1 : -1), c.getCenter())
        .applyTransformation()
        .render(m_matrix, {int16_t(m_matrix.getWidth() / 2), int16_t(m_matrix.getHeight() / 2)})
    ;
}

uint16_t FancyDemoPart6::getFrameCount() const
{
    return 180 / degreesPerFrame + 1;
}

void FancyDemoPart6::initLines()
{
    const int offset = 2;
    const int length = std::max(m_matrix.getHeight(), m_matrix.getWidth());

    const uint8_t s = 180;
    const uint8_t v = 255;
    Line rr(offset, 0, length, 0, CHSV(0, s, v));
    Line ll(-offset, 0, -length, 0, CHSV(32, s, v));
    Line tt(0, offset, 0, length, CHSV(64, s, v));
    Line bb(0, -offset, 0, -length, CHSV(96, s, v));
    Line tr(offset, offset, length, length, CHSV(128, s, v));
    Line tl(-offset, offset, -length, length, CHSV(160, s, v));
    Line br(offset, -offset, length, -length, CHSV(192, s, v));
    Line bl(-offset, -offset, -length, -length, CHSV(226, s, v));

    m_lines = rr
            .merged(ll)
            .merged(tt)
            .merged(bb)
            .merged(tr)
            .merged(tl)
            .merged(br)
            .merged(bl);
}

}
