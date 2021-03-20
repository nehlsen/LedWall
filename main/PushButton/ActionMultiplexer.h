#ifndef LEDWALL_ACTIONMULTIPLEXER_H
#define LEDWALL_ACTIONMULTIPLEXER_H

#include "Actions.h"

namespace LedWall {

class ModeController;

class ActionMultiplexer
{
public:
    explicit ActionMultiplexer(ModeController *modeController);

    void triggerAction(Action action);

private:
    ModeController *m_modeController;
};

}

#endif //LEDWALL_ACTIONMULTIPLEXER_H
