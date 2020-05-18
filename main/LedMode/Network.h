#ifndef LEDWALL_NETWORK_H
#define LEDWALL_NETWORK_H

#include "LedMode.h"
#include <GfxPrimitive.h>

class Network : public LedMode
{
public:
    explicit Network(LedMatrix& matrix);
    ~Network() override;

    void update() override;

protected:
    QueueHandle_t m_receiveQueue;
    TaskHandle_t m_udpServerTask;

    bool readQueue();
    GfxPrimitive m_buffer;
};

#endif //LEDWALL_NETWORK_H
