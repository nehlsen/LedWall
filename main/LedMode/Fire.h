#ifndef LEDWALL_FIRE_H
#define LEDWALL_FIRE_H

/**
 * adaptation of Fire2012WithPalette by Mark Kriegsman
 * https://github.com/FastLED/FastLED/blob/master/examples/Fire2012WithPalette/Fire2012WithPalette.ino
 */

#include "LedMode.h"

namespace LedWall {
namespace Mode {

class Fire : public LedMode
{
public:
    Fire(LedMatrix& matrix);
    ~Fire() override;

    bool update() override;

    void readOptions(cJSON *root) override;
    bool writeOptions(cJSON *root) override;

protected:
    bool isReady() const;

    // COOLING: How much does the air cool as it rises?
    // Less cooling = taller flames.  More cooling = shorter flames.
    // Default 55, suggested range 20-100
    uint8_t m_cooling = 55;

    // SPARKING: What chance (out of 255) is there that a new spark will be lit?
    // Higher chance = more roaring fire.  Lower chance = more flickery fire.
    // Default 120, suggested range 50-200.
    uint8_t m_sparking = 120;

    CRGBPalette16 m_palette = HeatColors_p;

    uint8_t *m_heat = nullptr;
    uint8_t *heat(uint8_t x, uint8_t y);

    void updateColumn(uint8_t x);
};

} // namespace Mode
} // namespace LedWall

#endif //LEDWALL_FIRE_H
