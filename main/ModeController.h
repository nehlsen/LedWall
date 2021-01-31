#ifndef LEDWALL_MODECONTROLLER_H
#define LEDWALL_MODECONTROLLER_H

#include <list>
#include <stdint.h>
#include <string>

class CRGB;
class LedMatrix;
class cJSON;

namespace LedWall {
namespace Mode {
class LedMode;
} // namespace Mode

class ModeController
{
public:
    explicit ModeController();

    void setPower(bool power);
    bool getPower() const;
    void triggerSystemReboot();

    void setBrightness(uint8_t brightness);
    uint8_t getBrightness() const;

    bool setModeByName(const std::string &name);
    bool setModeByIndex(int modeIndex);
    int getModeIndex() const; //!< get index of currently active mode
    std::string getModeName() const; //!< get name of currently active mode

    void savePreset(const std::string &presetName);
    bool loadPreset(const std::string &presetName);
    static void deletePreset(const std::string &presetName);
    static void deleteAllPresets();

    bool setModeOptions(cJSON *optionsObject);
    void getModeOptions(cJSON *optionsObject) const;
    bool resetModeOptions();

    Mode::LedMode *getLedMode() const;

protected:
    CRGB* m_leds;
    LedMatrix* m_matrix;

    bool m_power = true;
    int m_modeIndex = -1;
    Mode::LedMode *m_ledMode = nullptr;
    void updateMode(int newModeIndex, Mode::LedMode *newMode);

    static int getModeIndex(const std::string &name);

    static void setLedUpdateTaskEnabled(bool enabled);
    void turnAllLedsOff();

};

} // namespace LedWall

#endif //LEDWALL_MODECONTROLLER_H
