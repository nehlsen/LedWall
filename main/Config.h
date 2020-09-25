#ifndef LEDWALL_CONFIG_H
#define LEDWALL_CONFIG_H

#include <ConfigProperty.h>

namespace LedWall
{

class ModeController;

struct Config
{
    enum PowerOnMode {
        ALWAYS_OFF = 0,
        ALWAYS_ON = 1,
        RECOVER_LAST = 2,
    };
    static const int POWER_STATE_FALLBACK;
    static const int LED_MODE_AUTO_RESTORE_LAST;
    static const int LED_MODE_FALLBACK;

    static void init();
    static EBLi::ConfigProperty *matrixWidth();
    static EBLi::ConfigProperty *matrixHeight();
    static EBLi::ConfigProperty *brightness();
    static EBLi::ConfigProperty *powerOnMode();
    static EBLi::ConfigProperty *powerLastState();
    static EBLi::ConfigProperty *ledModeAutoRestore();
    static EBLi::ConfigProperty *ledModeLast();

    static bool isPoweredOnBoot();
    static int bootIntoLedMode();
};

}

#endif //LEDWALL_CONFIG_H
