#ifndef LEDWALL_MODEOPTIONSPERSISTER_H
#define LEDWALL_MODEOPTIONSPERSISTER_H

namespace LedWall {
namespace Mode {
class LedMode;
} // namespace Mode

class ModeOptionsPersister
{
public:
    bool saveOptions(Mode::LedMode *ledMode, const char *modeName);
    bool loadOptions(Mode::LedMode *ledMode, const char *modeName);
    void resetOptions(Mode::LedMode *ledMode, const char *modeName);
};

} // namespace LedWall

#endif //LEDWALL_MODEOPTIONSPERSISTER_H
