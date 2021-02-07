#ifndef LEDWALL_CONFIG_H
#define LEDWALL_CONFIG_H

#include <ConfigProperty.h>

namespace LedWall {

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
    static EBLi::config::ConfigProperty *matrixWidth();
    static EBLi::config::ConfigProperty *matrixHeight();
    static EBLi::config::ConfigProperty *brightness();
    static EBLi::config::ConfigProperty *powerOnMode();
    static EBLi::config::ConfigProperty *powerLastState();
    static EBLi::config::ConfigProperty *ledModeAutoRestore();
    //! last used led-mode (will be restored on power up)
    static EBLi::config::ConfigProperty *ledModeLast();

    static bool isPoweredOnBoot();
    static int bootIntoLedMode();
};

}

#endif //LEDWALL_CONFIG_H
