#ifndef LEDWALL_PRESETCHANGER_H
#define LEDWALL_PRESETCHANGER_H

#include <vector>
#include <string>
#include <cstdint>

namespace LedWall {

class ModeController;
typedef std::vector<std::string> PresetNameList;

class PresetChanger
{
public:
    explicit PresetChanger(ModeController *modeController);

    void addPreset(const std::string &presetName);
    void removePreset(const std::string &presetName);
    void clearPresets();
    PresetNameList getPresets() const;

    void setRandomEnabled(bool enabled);
    bool isRandomEnabled() const;

    /**
     * activates the next preset
     *
     * depending on whether random is enabled the next preset in sequence is activated or one at random
     */
    void activateNextPreset();

    void setAutoChangeDelay(uint16_t seconds);
    uint16_t getAutoChangeDelay() const;

    void setAutoChangeEnabled(bool enabled);
    bool isAutoChangeEnabled() const;

private:
    ModeController *m_modeController;

    PresetNameList m_presets;
    PresetNameList::const_iterator m_currentPreset;
    bool m_isRandomEnabled = false;
    uint16_t m_autoChangeDelay = 180;
    bool m_isAutoChangeEnabled = false;

    void activateNextPresetInSequence();
    void activateNextPresetRandom();
};

}

#endif //LEDWALL_PRESETCHANGER_H
