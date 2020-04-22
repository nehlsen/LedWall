#ifndef LEDWALL_MODECONTROLLER_H
#define LEDWALL_MODECONTROLLER_H

#include <list>

class CRGB;
class LedMode;
class ConfigManager;

typedef void (*change_handler_t)();

class ModeController
{
public:
    explicit ModeController(ConfigManager *configManager);

    void setPower(bool power);
    bool getPower() const;

    bool setModeIndex(int modeIndex);
    int getModeIndex() const;

    LedMode *getLedMode() const;

    // called every time something changes, e.g. power-on/off, mode
    void setChangeHandler(change_handler_t change_handler);

protected:
    CRGB* m_ledsWithSafety;
    CRGB* m_leds;

    ConfigManager *m_configManager;

    bool m_power = true;
    int m_modeIndex = -1;
    LedMode *m_ledMode = nullptr;

    void onChanged();
    change_handler_t m_change_handler = nullptr;

    static void setLedUpdateTaskEnabled(bool enabled);
    static void turnAllLedsOff();
};

#endif //LEDWALL_MODECONTROLLER_H
