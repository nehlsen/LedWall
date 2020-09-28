#include <Line.h>
#include <Rect.h>
#include <Canvas.h>
#include "FancyDemo.h"

namespace LedWall {
namespace Mode {

bool FancyDemo::update()
{
    m_matrix.clear(false);
//    m_matrix.fade(192);

    uint16_t remainingFrames = 0;
    remainingFrames += part1(m_frame);
    remainingFrames += part2((int16_t)m_frame - remainingFrames);
    remainingFrames += part3((int16_t)m_frame - remainingFrames);
    remainingFrames += part4((int16_t)m_frame - remainingFrames);
    remainingFrames += part5((int16_t)m_frame - remainingFrames);
    remainingFrames += part6a((int16_t) m_frame - remainingFrames);
    remainingFrames += part6b((int16_t) m_frame - remainingFrames);

    if ((int16_t)m_frame - remainingFrames >= 0) {
        m_frame = 0;
    } else {
        m_frame++;
    }

    return true;
}

int16_t FancyDemo::part1(int16_t frame)
{
    const uint16_t frameCount = m_matrix.getWidth() / 2 - 1;
    if (frame < 0 || frame >= frameCount) return frameCount;

    uint8_t x0 = std::round(m_matrix.getWidth() / 2.0 - 1.5);
    uint8_t x1 = x0 + 2;

    uint8_t y0 = m_matrix.getHeight() / 2;
    uint8_t y1 = y0 - 1;

    x0 -= frame;
    x1 += frame;

    // bottom
    Line(x0, y0, x1, y0, CRGB::White).render(m_matrix);
    // top
    Line(x0, y1, x1, y1, CRGB::White).render(m_matrix);

    return frameCount;
}

int16_t FancyDemo::part2(int16_t frame)
{
    const uint16_t frameCount = m_matrix.getHeight() / 2;
    if (frame < 0 || frame >= frameCount) return frameCount;

    uint8_t x0 = 0;
    uint8_t x1 = m_matrix.getWidth() - 1;

    uint8_t y0 = m_matrix.getHeight() / 2 - 1;
    uint8_t y1 = m_matrix.getHeight() / 2;

    y0 -= frame;
    y1 += frame;

    // bottom
    Line(x0, y0, x1, y0, CRGB::White).render(m_matrix);
    // top
    Line(x0, y1, x1, y1, CRGB::White).render(m_matrix);

    return frameCount;
}

int16_t FancyDemo::part3(int16_t frame)
{
    const uint16_t frameCount = m_matrix.getHeight() / 2;
    if (frame < 0 || frame >= frameCount) return frameCount;

    uint8_t x = m_matrix.getWidth() / 2 - 1 - frame;
    uint8_t y = m_matrix.getHeight() / 2 - 1 - frame;

    uint8_t w = 1 + 2*frame;
    uint8_t h = 1 + 2*frame;

    Rect(x, y, w, h, CRGB::White).render(m_matrix);

    return frameCount;
}

int16_t FancyDemo::part4(int16_t frame)
{
    const uint16_t frameCount = (m_matrix.getHeight() - 10) * (2.0/3.0);
    if (frame < 0 || frame >= frameCount) return frameCount;

    uint8_t x = (m_matrix.getWidth() - m_matrix.getHeight()) / 2 + frame;
    uint8_t y = frame;

    uint8_t w = m_matrix.getHeight() - 2*frame - 1;
    uint8_t h = m_matrix.getHeight() - 2*frame - 1;

    Rect(x, y, w, h, CRGB::White).render(m_matrix);

    return frameCount;
}

int16_t FancyDemo::part5(int16_t frame)
{
    // 360 / 8 = 45
    // 360 / 9 = 40
    // 360 / 10 = 36
    // 360 / 12 = 30
    // 360 / 15 = 24

    const uint8_t degreesPerFrame = 15;
    const uint16_t frameCount = 360 / degreesPerFrame;
    if (frame < 0 || frame >= frameCount) return frameCount;

    uint8_t cubeSize = 9;

    uint8_t x = 0;
    uint8_t y = 0;

    uint8_t w = cubeSize;
    uint8_t h = cubeSize;

    Canvas c(Rect(x, y, w, h, CRGB::White).pixels());
    c.setRotation((frame+1) * degreesPerFrame, c.getCenter())
     .applyTransformation()
//     .render(m_matrix)
     .render(m_matrix, {int16_t((m_matrix.getWidth() - cubeSize) / 2), int16_t((m_matrix.getHeight() - cubeSize) / 2)})
//     .renderCentered(m_matrix)
     ;

    return frameCount;
}

int16_t FancyDemo::part6a(int16_t frame)
{
    return part6_abs(frame, false);
}

int16_t FancyDemo::part6b(int16_t frame)
{
    return part6_abs(frame, true);
}

int16_t FancyDemo::part6_abs(int16_t frame, bool forward)
{
    const uint8_t degreesPerFrame = 10;
    const uint16_t frameCount = 180 / degreesPerFrame + 1;
    if (frame < 0 || frame >= frameCount) return frameCount;

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

    GfxPrimitive lines = rr
            .merged(ll)
            .merged(tt)
            .merged(bb)
            .merged(tr)
            .merged(tl)
            .merged(br)
            .merged(bl);

    Canvas c(lines.pixels());
    c.setRotation((frame) * degreesPerFrame * (forward ? 1 : -1), c.getCenter())
            .applyTransformation()
//     .render(m_matrix)
            .render(m_matrix, {int16_t(m_matrix.getWidth() / 2), int16_t(m_matrix.getHeight() / 2)})
//     .renderCentered(m_matrix)
            ;

    return frameCount;
}

} // namespace Mode
} // namespace LedWall
