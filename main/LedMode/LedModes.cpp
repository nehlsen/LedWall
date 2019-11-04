#include "LedModes.h"

#include "LedModeStatus.h"
#include "LedModeSample.h"
#include "LedModeHsiboy.h"

LedMode *createModeStatus(CRGB *leds, int ledCount) { return new LedModeStatus(leds, ledCount); }
LedMode *createModeSample(CRGB *leds, int ledCount) { return new LedModeSample(leds, ledCount); }
LedMode *createModeHsiboy(CRGB *leds, int ledCount) { return new LedModeHsiboy(leds, ledCount); }

LedModeDef_t modeStatusDef {"Status", &createModeStatus};
LedModeDef_t modeSampleDef {"Sample", &createModeSample};
LedModeDef_t modeHsiboyDef {"Hsiboy", &createModeHsiboy};

std::vector<LedModeDef_t> LedModes {
        modeStatusDef,
//        {"Status", &createModeStatus},
        modeSampleDef,
//        {"Sample", &createModeSample},
        modeHsiboyDef,
//        {"Hsiboy", &createModeHsiboy},
};
