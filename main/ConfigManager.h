#ifndef LEDWALL_CONFIGMANAGER_H
#define LEDWALL_CONFIGMANAGER_H

#include <nvs.h>

class ConfigManager
{
public:
    bool open();
    void close();
    bool commit();

    void setAutoCommitEnabled(bool enabled = true);
    bool isAutoCommitEnabled() const;

    // fixme refactor to template function?
    int32_t getIntVal(const char *key, int32_t defaultValue);
    bool setIntVal(const char *key, int32_t value);

    int32_t getRestartCounter();

    enum AutoPowerOn {
        ALWAYS_OFF = 0,
        ALWAYS_ON = 1,
        RECOVER_LAST = 2,
    };
    void setPowerOnResetMode(AutoPowerOn mode); // whether to remember or auto on/off
    AutoPowerOn getPowerOnResetMode();
    void setPowerState(bool currentPowerState); // remember current power state
    bool isPoweredOnBoot();

//    void setBootIntoMode(); // set mode to boot into, disables restore
//    void setAutoRestoreModeEnabled(); // enable auto restore mode
    void setLedModeAutoRestore(int autoRestoreMode); // -1 auto restore, >=0 boot in this mode (ignores remembered mode)
    int getLedModeAutoRestore();
    void setLedMode(int currentModeIndex); // remember current mode
    int getBootIntoMode();

protected:
    bool m_autoCommitEnabled = true;
    bool autoCommit();

    nvs_handle_t m_nvsHandle = 0;

    void updateRestartCounter();
    int32_t m_restartCounter = 0;
};

#endif //LEDWALL_CONFIGMANAGER_H
