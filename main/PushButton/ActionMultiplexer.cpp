#include "ActionMultiplexer.h"

namespace LedWall {

void ActionMultiplexer::triggerAction(Action action)
{
    for (auto actionLister : m_actionListeners) {
        actionLister->handleAction(action);
    }
}

void ActionMultiplexer::addActionListener(ActionListener *actionListener)
{
    m_actionListeners.push_back(actionListener);
}

}
