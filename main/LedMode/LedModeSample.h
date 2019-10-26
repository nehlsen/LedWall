#ifndef LEDWALL_LEDMODESAMPLE_H
#define LEDWALL_LEDMODESAMPLE_H

#include "FastLED.h"
#include "LedMode.h"

class LedModeSample: public LedMode
{
public:
    using LedMode::LedMode;

    void update() override;

protected:
    void changePalettePeriodically();

    // This function fills the palette with totally random colors.
    void setupTotallyRandomPalette();

    // This function sets up a palette of black and white stripes,
    // using code.  Since the palette is effectively an array of
    // sixteen CRGB colors, the various fill_* functions can be used
    // to set them up.
    void setupBlackAndWhiteStripedPalette();

    // This function sets up a palette of purple and green stripes.
    void setupPurpleAndGreenPalette();

    CRGBPalette16 m_currentPalette;
    TBlendType m_currentBlending;
};

#endif //LEDWALL_LEDMODESAMPLE_H
