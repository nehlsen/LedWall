#ifndef LEDWALL_LEDMODES_H
#define LEDWALL_LEDMODES_H

#include <vector>
#include "LedMode.h"

typedef LedMode* (LedModeFactory)(LedMatrix& matrix);

typedef struct {
    const char name[16];
    LedModeFactory *const factory;
} LedModeDef_t;

extern std::vector<LedModeDef_t> LedModes;
typedef std::vector<LedModeDef_t>::size_type t_LedModes_size;

#endif //LEDWALL_LEDMODES_H
