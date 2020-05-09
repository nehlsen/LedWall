#ifndef LEDWALL_NETWORK_H
#define LEDWALL_NETWORK_H

#include "LedMode.h"

class Network : public LedMode
{
public:
    using LedMode::LedMode;

    void update() override;
};

#endif //LEDWALL_NETWORK_H
