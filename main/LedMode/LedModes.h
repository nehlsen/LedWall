#ifndef LEDWALL_LEDMODES_H
#define LEDWALL_LEDMODES_H

#include <vector>
#include "LedMode.h"

namespace LedWall {
namespace Mode {

typedef LedMode* (LedModeFactory)(LedMatrix& matrix);

typedef struct {
    const char name[16];
    LedModeFactory *const factory;
} LedModeDef_t;

extern std::vector<LedModeDef_t> LedModes;
typedef std::vector<LedModeDef_t>::iterator t_LedModes_iterator;
typedef std::vector<LedModeDef_t>::size_type t_LedModes_size;

} // namespace Mode
} // namespace LedWall

#endif //LEDWALL_LEDMODES_H
