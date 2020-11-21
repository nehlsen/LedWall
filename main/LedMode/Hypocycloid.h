#ifndef LEDWALL_HYPOCYCLOID_H
#define LEDWALL_HYPOCYCLOID_H

#include "LedMode.h"
#include <Canvas.h>

namespace LedWall::Mode {

class Hypocycloid : public LedMode
{
public:
    using LedMode::LedMode;

    bool update() override;

    Pixels draw(int m);

    int m_testParam = 0;
};

}

#endif //LEDWALL_HYPOCYCLOID_H
