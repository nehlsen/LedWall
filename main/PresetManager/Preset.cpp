#include "Preset.h"
#include <cJSON.h>

namespace LedWall {

Preset::~Preset()
{
    // FIXME we do not own this! who does?!
//    cJSON_Delete(m_modeOptions);
}

const std::string &Preset::getPresetName() const
{
    return m_presetName;
}

void Preset::setPresetName(const std::string &presetName)
{
    m_presetName = presetName;
}

const std::string &Preset::getModeName() const
{
    return m_modeName;
}

void Preset::setModeName(const std::string &modeName)
{
    m_modeName = modeName;
}

bool Preset::hasModeOptions() const
{
    return nullptr != m_modeOptions;
}

cJSON *Preset::getModeOptions() const
{
    return m_modeOptions;
}

void Preset::setModeOptions(cJSON *modeOptions)
{
    if (hasModeOptions()) {
        cJSON_Delete(m_modeOptions);
    }

    m_modeOptions = modeOptions;
}

}
