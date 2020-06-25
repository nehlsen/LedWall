#include "MatesDemo.h"
#include <Line.h>
#include <Text.h>

namespace LedWall {
namespace Mode {

bool MatesDemo::update()
{
    uint8_t currentSeconds = (millis() / 1000) % 60;

    if(m_lastUpdate != currentSeconds) {
        m_lastUpdate = currentSeconds;

        m_matrix.clear(true);
//        drawSample(currentSeconds % countSamples);
        drawSample(7);
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

    switch (index) {
        case 0:
            Line(bottomLeft, bottomRight).setColor(CRGB::Red).render(m_matrix);
            drawSampleLetter("B");
            break;
        case 1:
            Line(topLeft, topRight).setColor(CRGB::Red).render(m_matrix);
            drawSampleLetter("T");
            break;

        case 2:
            Line(bottomLeft, topLeft).setColor(CRGB::Red).render(m_matrix);
            drawSampleLetter("L");
            break;
        case 3:
            Line(bottomRight, topRight).setColor(CRGB::Red).render(m_matrix);
            drawSampleLetter("R");
            break;

        case 4:
            Line(bottomLeft, topRight).setColor(CRGB::Red).render(m_matrix);
            break;
        case 5:
            Line(bottomRight, topLeft).setColor(CRGB::Red).render(m_matrix);
            break;

        case 6:
            Line(bottomLeft, bottomRight).setGradient(CRGBPalette16(CRGB::Red, CRGB::Green, CRGB::Blue)).render(m_matrix);
            drawSampleLetter("G");
            break;

        case 7:
            ::Text smplLetter("R", CRGB::Red);
            const int text_x = (m_matrix.getWidth() - smplLetter.getSize().width) / 2;
            const int text_y = (m_matrix.getHeight() - smplLetter.getSize().height) / 2;

//            smplLetter.setX(text_x);
//            smplLetter.setY(text_y);
            smplLetter.setCanvas(text_x, text_y);

//            smplLetter.setRotation(90);
            smplLetter.render(m_matrix);
            break;

//        case 6:
//            // hexagon optimal angled line
//            Line({2, top}, {5, 0}).setColor(CRGB::Red).render(m_matrix);
//            break;
//        case 7:
//            // hexagon optimal angled line
//            Line({2, 0}, {5, top}).setColor(CRGB::Red).render(m_matrix);
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
