#ifndef LEDWALL_EVENTS_H
#define LEDWALL_EVENTS_H

#include "esp_event.h"

ESP_EVENT_DECLARE_BASE(LEDWALL_EVENTS);

enum {
    LEDWALL_EVENT_POWER_CHANGED, // arg: (bool) isPowered
    LEDWALL_EVENT_MODE_CHANGED   // arg: (int) modeIndex
};

#endif //LEDWALL_EVENTS_H
