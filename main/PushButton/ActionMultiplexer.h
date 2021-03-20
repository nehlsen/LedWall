#ifndef LEDWALL_ACTIONMULTIPLEXER_H
#define LEDWALL_ACTIONMULTIPLEXER_H

#include <vector>
#include "Actions.h"
#include "ActionListener.h"

namespace LedWall {

class ActionMultiplexer
{
public:
    void triggerAction(Action action);
    void addActionListener(ActionListener *actionListener);

private:
    std::vector<ActionListener*> m_actionListeners;
};

}

#endif //LEDWALL_ACTIONMULTIPLEXER_H
