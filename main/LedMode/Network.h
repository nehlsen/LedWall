#ifndef LEDWALL_NETWORK_H
#define LEDWALL_NETWORK_H

#include "LedMode.h"
#include <GfxPrimitive.h>

class Network : public LedMode
{
public:
    explicit Network(LedMatrix& matrix);

    void update() override;

    void onNetworkData(char *data, int length);

protected:
    GfxPrimitive m_buffer;

    void handlePixelStreamData(char *data, int length);
};

#endif //LEDWALL_NETWORK_H
