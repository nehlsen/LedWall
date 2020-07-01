#ifndef LEDWALL_MODECONTROLLER_H
#define LEDWALL_MODECONTROLLER_H

#include <list>
#include <stdint.h>
#include <string>

class CRGB;
class LedMatrix;

namespace LedWall {
namespace Mode {
class LedMode;
} // namespace Mode
class ConfigManager;
class ModeOptionsPersister;

class ModeController
{
public:
    explicit ModeController(ConfigManager *configManager);

    void setPower(bool power);
    bool getPower() const;
    void triggerSystemReboot();

    void setBrightness(uint8_t brightness);
    uint8_t getBrightness() const;

    bool setModeByName(const std::string &name);
    bool setModeByIndex(int modeIndex);
    int getModeIndex() const;

    Mode::LedMode *getLedMode() const;

protected:
    CRGB* m_leds;
    LedMatrix* m_matrix;

    ConfigManager *m_configManager;
    ModeOptionsPersister *m_modeOptionsPersister;

    bool m_power = true;
    int m_modeIndex = -1;
    Mode::LedMode *m_ledMode = nullptr;

    static void setLedUpdateTaskEnabled(bool enabled);
    void turnAllLedsOff();

};

} // namespace LedWall

#endif //LEDWALL_MODECONTROLLER_H
