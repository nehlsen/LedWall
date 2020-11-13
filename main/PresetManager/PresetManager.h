#ifndef LEDWALL_PRESETMANAGER_H
#define LEDWALL_PRESETMANAGER_H

#include "Preset.h"
#include <cstdio>

class cJSON;

namespace LedWall {
namespace Mode {
class LedMode;
}

class PresetManager
{
public:
    virtual ~PresetManager();

    PresetList getPresets();

    bool hasPreset(const std::string &presetName);
    Preset getPreset(const std::string &presetName);
    void savePreset(const std::string &presetName, const char *modeName, Mode::LedMode *ledMode);
    void deletePreset(const std::string &presetName);
    void deleteAllPresets();

private:
    FILE *m_presetsFile = nullptr;
    bool openPresetsFile(bool openForWriting = false);
    void closePresetsFile();

    PresetList m_presetList;
    bool readPresetsFromFile();
    bool writePresetsToFile();

    static Preset readPreset(cJSON *presetJson);
    static cJSON *writePreset(const Preset &preset);
    static cJSON *writePreset(const char *presetName, const char *modeName, cJSON *modeOptions);

    PresetList::iterator findPresetByName(const std::string &presetName);
    void createPreset(const std::string &presetName, const char *modeName, Mode::LedMode *ledMode);
};

}

#endif //LEDWALL_PRESETMANAGER_H
