#ifndef LEDWALL_LEDCONTROLLER_H
#define LEDWALL_LEDCONTROLLER_H

#include <list>

class LedMode;
class ConfigManager;

typedef void (*change_handler_t)();

class LedController
{
public:
    explicit LedController(ConfigManager *configManager);

    void setPower(bool power);
    bool getPower() const;

    bool setModeIndex(int modeIndex);
    int getModeIndex() const;

    LedMode *getLedMode() const;

    // called every time something changes, e.g. power-on/off, mode
    void setChangeHandler(change_handler_t change_handler);

protected:
    ConfigManager *m_configManager;

    bool m_power = true;
    int m_modeIndex = -1;
    LedMode *m_ledMode = nullptr;

    void onChanged();
    change_handler_t m_change_handler = nullptr;

    static void setLedUpdateTaskEnabled(bool enabled);
    static void turnAllLedsOff();
};

#endif //LEDWALL_LEDCONTROLLER_H
