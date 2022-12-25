#ifndef LEDWALL_FANCYDEMO_H
#define LEDWALL_FANCYDEMO_H

#include "FancyScript.h"

namespace LedWall::Mode {

class FancyDemo : public FancyScript
{
public:
    explicit FancyDemo(LedMatrix& matrix);
};

}

#endif //LEDWALL_FANCYDEMO_H
