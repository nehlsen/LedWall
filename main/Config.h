#ifndef LEDWALL_CONFIG_H
#define LEDWALL_CONFIG_H

#include <ConfigProperty.h>

namespace LedWall
{

class ModeController;

struct Config
{
    static void init(ModeController *controller);
    static EBLi::ConfigProperty *matrixWidth();
    static EBLi::ConfigProperty *matrixHeight();
    static EBLi::ConfigProperty *brightness();
};

}

#endif //LEDWALL_CONFIG_H
