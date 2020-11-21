#include "Hypocycloid.h"

#include <numeric>
#include <cmath>
#include <QDebug>

// https://en.wikipedia.org/wiki/Hypocycloid
// https://en.wikipedia.org/wiki/Epicycloid

namespace LedWall::Mode {

bool Hypocycloid::update()
{
    m_matrix.clear(false);

    m_testParam++;
    Canvas(draw(m_testParam)).render(m_matrix, {4+7, 7});

    if (m_testParam > 200) {
        m_testParam = 0;
    }

    return true;
}

Pixels Hypocycloid::draw(int m)
{
    Pixels pixels;

    Canvas canvas;

    const int k = 3; // how many edges
    const int radiusInner = 2;
    const int radiusOuter = k * radiusInner;
    const float maxTheta = 2.0 * PI * (std::lcm(radiusInner, radiusOuter) / (float)radiusOuter);

//    const int p = 2; // more equals greater loop; negative leads to more "qube" shape
    const float p = (m/10.0) - 10; // negative 10 to positive 10 in 0.1 steps

    for (float theta = 0; theta <= maxTheta; theta+=0.1) {
        int x = radiusInner * (k-1) * std::cos(theta) + (radiusInner + p) * std::cos((k - 1) * theta);
        int y = radiusInner * (k-1) * std::sin(theta) - (radiusInner + p) * std::sin((k - 1) * theta);

        pixels.push_back(Pixel(x, y, CRGB(255, 0, 0)));
    }

    return pixels;
}

}
