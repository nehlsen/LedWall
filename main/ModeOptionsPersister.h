#ifndef LEDWALL_MODEOPTIONSPERSISTER_H
#define LEDWALL_MODEOPTIONSPERSISTER_H

#include <cstdio>

namespace LedWall {
namespace Mode {
class LedMode;
} // namespace Mode

class ModeOptionsPersister
{
public:
    static bool save(Mode::LedMode *ledMode, const char *modeName);
    static bool load(Mode::LedMode *ledMode, const char *modeName);
    static bool reset(Mode::LedMode *ledMode, const char *modeName);

protected:
    ModeOptionsPersister();
    ~ModeOptionsPersister();

    bool saveOptions(Mode::LedMode *ledMode, const char *modeName);
    bool loadOptions(Mode::LedMode *ledMode, const char *modeName);
    bool resetOptions(Mode::LedMode *ledMode, const char *modeName);

    char *m_cfgFileName = nullptr;
    char *m_readBuffer = nullptr;
    char *m_writeBuffer = nullptr;
    FILE *m_cfgFile = nullptr;
};

} // namespace LedWall

#endif //LEDWALL_MODEOPTIONSPERSISTER_H
