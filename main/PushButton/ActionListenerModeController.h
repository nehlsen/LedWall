#ifndef LEDWALL_ACTIONLISTENERMODECONTROLLER_H
#define LEDWALL_ACTIONLISTENERMODECONTROLLER_H

#include "ActionListener.h"

namespace LedWall {

class ModeController;

class ActionListenerModeController : public ActionListener
{
public:
    explicit ActionListenerModeController(ModeController *modeController);

    bool handleAction(Action action) override;

private:
    ModeController *m_modeController;
};

}

#endif //LEDWALL_ACTIONLISTENERMODECONTROLLER_H
