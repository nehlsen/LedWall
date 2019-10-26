#ifndef LEDWALL_LEDCONTROLLER_H
#define LEDWALL_LEDCONTROLLER_H

typedef void (*change_handler_t)();

class LedController
{
public:
    LedController();

    void setPower(bool power);
    bool getPower();

    enum Mode {
        LedModeStatus = 0, // display wlan, mqtt status
        LedModeSample,
    };

    void setMode(Mode mode);
    Mode getMode();

    // called every time something changes, e.g. power-on/off, mode
    void setChangeHandler(change_handler_t change_handler);

protected:
    bool m_power = true;
    Mode m_mode = LedModeStatus;

    void onChanged();
    change_handler_t m_change_handler = nullptr;
};

#endif //LEDWALL_LEDCONTROLLER_H
