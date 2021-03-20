#ifndef LEDWALL_ACTIONLISTENER_H
#define LEDWALL_ACTIONLISTENER_H

#include "Actions.h"

namespace LedWall {

class ActionListener
{
public:
    virtual bool handleAction(Action action) = 0;
};

}

#endif //LEDWALL_ACTIONLISTENER_H
