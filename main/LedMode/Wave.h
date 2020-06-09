#ifndef LEDWALL_WAVE_H
#define LEDWALL_WAVE_H

#include <vector>
#include <Circle.h>
#include <Rect.h>
#include "LedMode.h"

namespace LedWall {
namespace Mode {

class Wave : public LedMode
{
public:
//    using LedMode::LedMode;
    explicit Wave(LedMatrix &matrix);

    void readOptions(cJSON *root) override;
    bool writeOptions(cJSON *root) override;

    enum WaveMode {
        WaveModeHorizontal,
        WaveModeVertical,
        WaveModeRadialCircle,
        WaveModeRadialRect,
    };

    WaveMode getWaveMode() const;
    void setWaveMode(WaveMode waveMode);

    enum WaveDirection {
        WaveDirectionForward,
        WaveDirectionReverse,
    };

    WaveDirection getWaveDirection() const;
    void setWaveDirection(WaveDirection waveDirection);

    // peak to peak in pixels
    int getWaveLength() const; // TODO uint8_t should suffice
    void setWaveLength(int waveLength);

    // how many pixels the wave-peak travels per tenth-of-a-second
    int getSpeed() const; // TODO uint8_t should suffice
    void setSpeed(int speed);

    std::pair<uint8_t, uint8_t> getModHue() const;
    void setModHue(uint8_t low, uint8_t high);
    std::pair<uint8_t, uint8_t> getModSaturation() const;
    void setModSaturation(uint8_t low, uint8_t high);
    std::pair<uint8_t, uint8_t> getModValue() const;
    void setModValue(uint8_t low, uint8_t high);

    bool update() override;

protected:
    WaveMode m_waveMode;
    WaveDirection m_waveDirection;
    int m_waveLength;
    int m_totalFrames;
    int m_speed;

    int m_frame = 0;

    double sineValue(int x, double offset) const;
    void drawWaveHorizontal(double offset);
    void drawWaveVertical(double offset);

    void drawCircles(double offset);
    void initCircles();
    void clearCircles();
    std::vector<Circle> m_circles;

    void drawRects(double offset);
    void initRects();
    void clearRects();
    std::vector<Rect> m_rects;

    uint8_t m_colorHueLow = 255;
    uint8_t m_colorHueHigh = 255;
    uint8_t m_colorSaturationLow = 255;
    uint8_t m_colorSaturationHigh = 255;
    uint8_t m_colorValueLow = 255;
    uint8_t m_colorValueHigh = 255;

    // find a point between low and high defined by factor
    //   if factor is 0.0 the point is low
    //   if factor is 1.0 the point is high
    //   if factor is 0.5 the point is exactly in the middle between low and high
    static uint8_t factoredConstrain(uint8_t low, uint8_t high, double factor);
};

} // namespace Mode
} // namespace LedWall

#endif //LEDWALL_WAVE_H
