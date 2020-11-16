#ifndef LEDWALL_PRESET_H
#define LEDWALL_PRESET_H

#include <vector>
#include <string>

class cJSON;

namespace LedWall {

class Preset
{
public:
    virtual ~Preset();

    const std::string &getPresetName() const;
    void setPresetName(const std::string &presetName);

    const std::string &getModeName() const;
    void setModeName(const std::string &modeName);

    bool hasModeOptions() const;
    [[nodiscard]] cJSON *getModeOptions() const;
    void setModeOptions(cJSON *modeOptions);

    static const uint8_t ValidNameLength = 16;
    static bool isValidName(const std::string &presetName);

private:
    std::string m_presetName;
    std::string m_modeName;
    cJSON *m_modeOptions = nullptr;
};

typedef std::vector<Preset> PresetList;

}

#endif //LEDWALL_PRESET_H
