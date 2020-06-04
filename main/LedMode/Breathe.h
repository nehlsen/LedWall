#ifndef LEDWALL_BREATHE_H
#define LEDWALL_BREATHE_H

// https://github.com/forkineye/ESPixelStick/blob/master/EffectEngine.cpp

  /*
   * Subtle "breathing" effect, works best with gamma correction on.
   *
   * The average resting respiratory rate of an adult is 12–18 breaths/minute.
   * We use 12 breaths/minute = 5.0s/breath at the default _effectDelay.
   * The tidal volume (~0.5l) is much less than the total lung capacity,
   * so we vary only between 75% and 100% of the set brightness.
   *
   * Per default, this is subtle enough to use with a flood, spot, ceiling or
   * even bedside light. If you want more variation, use the values given
   * below for a 33%/67% variation.
   *
   * In the calculation, we use some constants to make it faster:
   * 0.367879441 is: 1/e
   * 0.106364766 is: 0.25/(e-1/e)  [25% brightness variation, use 0.140401491 for 33%]
   * 0.75 is the offset [75% min brightness, use 0.67 for 67%]
   *
   * See also https://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
   * for a nice explanation of the math.
   */
// sin() is in radians, so 2*PI rad is a full period; compiler should optimize.

/*
float val = (exp(sin(millis()/(_effectDelay*5.0)*2*PI)) - 0.367879441) * 0.106364766 + 0.75;
setAll({_effectColor.r*val, _effectColor.g*val, _effectColor.b*val});
return _effectDelay / 40; // update every 25ms
*/

#include "LedMode.h"
#include <Circle.h>
#include <Rect.h>

namespace LedWall {
namespace Mode {

class Breathe : public LedMode
{
public:
    explicit Breathe(LedMatrix &matrix);

    bool update() override;

    void readOptions(cJSON *root) override;
    bool writeOptions(cJSON *root) override;

//    enum Shape {
//        ShapeCenteredCircle,
//        ShapeFullSizeRect,
//    };
//    void setShape(Shape shape);
//    void setShape(GfxPrimitive *shape);

    void setColor(const CRGB &color);

    /**
     * set variation in percent
     *
     * default is 25% variation -> configured color will range between 100% and 75% (100-25)
     * setting this to 33% will lead to a range from 100% to 67%
     *
     * this method updates m_offset and m_brightnessVariation
     *
     * @param percent 1-100 (other values will be silently dropped)
     */
    void setVariation(uint8_t percent);
    uint8_t getVariation() const;

    void setCycleTime(uint16_t cycleTime);

protected:
    Rect m_shape;

    CRGB m_color = CRGB(255, 255, 255);
    double m_offset = 0.75;
    double m_brightnessVariation = 0.106364766; // 0.25/(e-1/e)  -> 25% brightness variation
    uint16_t m_cycleTime = 5000; // 5seconds per breath
};

} // namespace Mode
} // namespace LedWall

#endif //LEDWALL_BREATHE_H
