#ifndef LEDWALL_CONFIGMANAGER_H
#define LEDWALL_CONFIGMANAGER_H

#include <nvs.h>
#include <string>

namespace LedWall {

 class [[deprecated]] ConfigManager
{
public:
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
    AutoPowerOn getPowerOnResetMode() const;
    void setPowerState(bool currentPowerState); // remember current power state
    bool isPoweredOnBoot() const;

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
};

} // namespace LedWall

#endif //LEDWALL_CONFIGMANAGER_H
