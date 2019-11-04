#ifndef LEDWALL_LEDCONTROLLER_H
#define LEDWALL_LEDCONTROLLER_H

#include <list>

class LedMode;

typedef void (*change_handler_t)();

class LedController
{
public:
    LedController();

    void setPower(bool power);
    bool getPower() const;

    bool setModeIndex(int modeIndex);
    int getModeIndex() const;

    LedMode *getLedMode() const;

    // called every time something changes, e.g. power-on/off, mode
    void setChangeHandler(change_handler_t change_handler);

protected:
    bool m_power = true;
    int m_modeIndex = -1;
    LedMode *m_ledMode = nullptr;

    void onChanged();
    change_handler_t m_change_handler = nullptr;

    static void setLedUpdateTaskEnabled(bool enabled);
    static void turnAllLedsOff();
};

#endif //LEDWALL_LEDCONTROLLER_H
