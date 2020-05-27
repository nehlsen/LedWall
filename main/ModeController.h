#ifndef LEDWALL_MODECONTROLLER_H
#define LEDWALL_MODECONTROLLER_H

#include <list>
#include <stdint.h>

class CRGB;
class LedMatrix;

namespace LedWall {
namespace Mode {
class LedMode;
} // namespace Mode
class ConfigManager;

class ModeController
{
public:
    explicit ModeController(ConfigManager *configManager);

    void setPower(bool power);
    bool getPower() const;
    void triggerSystemReboot();

    void setBrightness(uint8_t brightness);
    uint8_t getBrightness() const;

    bool setModeIndex(int modeIndex);
    int getModeIndex() const;

    Mode::LedMode *getLedMode() const;

protected:
    CRGB* m_leds;
    LedMatrix* m_matrix;

    ConfigManager *m_configManager;

    bool m_power = true;
    int m_modeIndex = -1;
    Mode::LedMode *m_ledMode = nullptr;

    static void setLedUpdateTaskEnabled(bool enabled);
    static void turnAllLedsOff();
};

} // namespace LedWall

#endif //LEDWALL_MODECONTROLLER_H
