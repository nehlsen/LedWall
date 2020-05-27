#ifndef LEDWALL_CONFIGMANAGER_H
#define LEDWALL_CONFIGMANAGER_H

#include <nvs.h>
#include <string>

namespace LedWall {

class ConfigManager
{
public:
    bool open();
    void close();
    bool commit();

    void setAutoCommitEnabled(bool enabled = true);
    bool isAutoCommitEnabled() const;

    // FIXME refactor to template function?
    int32_t getIntVal(const char *key, int32_t defaultValue) const;
    bool setIntVal(const char *key, int32_t value);
    // NOTE value is limited to 64 characters!
    std::string getStringVal(const char *key, const std::string &defaultValue) const;
    bool setStringVal(const char *key, const std::string &value);

    int32_t getRestartCounter();

    uint8_t getMatrixWidth() const;
    void setMatrixWidth(uint8_t width);
    uint8_t getMatrixHeight() const;
    void setMatrixHeight(uint8_t height);

    uint8_t getBrightness() const;
    void setBrightness(uint8_t brightness);

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
    int getLedModeAutoRestore() const;
    void setLedMode(int currentModeIndex); // remember current mode
    int getBootIntoMode() const;

    std::string getMqttBroker() const;
    void setMqttBroker(const std::string& brokerUrl);
    std::string getMqttDeviceTopic() const;
    void setMqttDeviceTopic(const std::string& topic);
    std::string getMqttGroupTopic() const;
    void setMqttGroupTopic(const std::string& topic);

protected:
    bool m_autoCommitEnabled = true;
    bool autoCommit();

    nvs_handle_t m_nvsHandle = 0;

    void updateRestartCounter();
    int32_t m_restartCounter = 0;
};

} // namespace LedWall

#endif //LEDWALL_CONFIGMANAGER_H
