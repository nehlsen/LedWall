#include "MatesDemo.h"
#include <Line.h>
#include <Text.h>
#include <GfxCanvas.h>
#include <Canvas.h>
#include <Rect.h>

namespace LedWall {
namespace Mode {

bool MatesDemo::update()
{
    const int countSamples = 13;
    uint8_t currentSeconds = (millis() / 1000) % 60;

    if(m_lastUpdate != currentSeconds) {
        m_lastUpdate = currentSeconds;

        m_matrix.clear(false);
        drawSample(currentSeconds % countSamples);
//        drawSample(7);
        return true;
    }

    return false;
}

void MatesDemo::drawSample(int index)
{
    const int16_t top = m_matrix.getHeight()-1;
    const int16_t right = m_matrix.getWidth()-1;

    const Point bottomLeft({0, 0});
    const Point bottomRight({right, 0});
    const Point topLeft({0, top});
    const Point topRight({right, top});

    const CRGB solidColor(CRGB::Red);
    const CRGBPalette16 gradient(CRGB::Red, CRGB::Green, CRGB::Blue);

    switch (index) {
        // straight lines (at bottom, top, left, right), solid color
        case 0:
            Line(bottomLeft, bottomRight).setColor(solidColor).render(m_matrix);
            drawSampleLetter("B");
            break;
        case 1:
            Line(topLeft, topRight).setColor(solidColor).render(m_matrix);
            drawSampleLetter("T");
            break;

        case 2:
            Line(bottomLeft, topLeft).setColor(solidColor).render(m_matrix);
            drawSampleLetter("L");
            break;
        case 3:
            Line(bottomRight, topRight).setColor(solidColor).render(m_matrix);
            drawSampleLetter("R");
            break;

        // diagonal lines, solid color
        case 4:
            Line(bottomLeft, topRight).setColor(solidColor).render(m_matrix);
            break;
        case 5:
            Line(bottomRight, topLeft).setColor(solidColor).render(m_matrix);
            break;

        // straight line, gradient
        case 6:
            Line(bottomLeft, bottomRight).setGradient(gradient).render(m_matrix);
            drawSampleLetter("G");
            break;

        // using canvas to rotate a text 90, 180 and 270 degrees clock-wise
        case 7:
        case 8:
        case 9: {
            Text smplLetter("R", solidColor);

            Canvas c(smplLetter.pixels());
            c.setRotation((index-6)*90, c.getCenter())
             .applyTransformation()
             .renderCentered(m_matrix);
            break;
        }

        case 10: {
            Text smpl("Text");
            smpl
                    .setGradient(gradient, Text::GradientModePerLetter)
                    .setX((m_matrix.getWidth() - smpl.getSize().width) / 2)
                    .setY((m_matrix.getHeight() - smpl.getSize().height) / 2)
                    .render(m_matrix);
            break;
        }

        case 11: {
            Text smpl("Text");
            smpl
                    .setGradient(gradient, Text::GradientModeOverlay)
                    .setX((m_matrix.getWidth() - smpl.getSize().width) / 2)
                    .setY((m_matrix.getHeight() - smpl.getSize().height) / 2)
                    .render(m_matrix);
            break;
        }

        case 12:
            Rect(0, 0, m_matrix.getWidth(), m_matrix.getHeight(), CRGB::White).setFillColor(CRGB::White).render(m_matrix);
            break;

//        case 6:
//            // hexagon optimal angled line
//            Line({2, top}, {5, 0}).setColor(solidColor).render(m_matrix);
//            break;
//        case 7:
//            // hexagon optimal angled line
//            Line({2, 0}, {5, top}).setColor(solidColor).render(m_matrix);
//            break;
    }
}

void MatesDemo::drawSampleLetter(char *c)
{
    if (m_matrix.getWidth() < 7 || m_matrix.getHeight() < 9) {
        return;
    }

    ::Text smplLetter(c, CRGB::Red);
    smplLetter.setX((m_matrix.getWidth() - smplLetter.getSize().width) / 2);
    smplLetter.setY((m_matrix.getHeight() - smplLetter.getSize().height) / 2);
    smplLetter.render(m_matrix);
}

} // namespace Mode
} // namespace LedWall
