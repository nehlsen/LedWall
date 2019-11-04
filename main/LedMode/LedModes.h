#ifndef LEDWALL_LEDMODES_H
#define LEDWALL_LEDMODES_H

#include <vector>

#include "LedMode.h"

typedef LedMode* (LedModeFactory)(CRGB *leds, int ledCount);

typedef struct {
    const char name[16];
    LedModeFactory *const factory;
} LedModeDef_t;

extern std::vector<LedModeDef_t> LedModes;

#endif //LEDWALL_LEDMODES_H
