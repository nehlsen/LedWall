#include "ActionListenerModeController.h"
#include "../ModeController.h"

namespace LedWall {

ActionListenerModeController::ActionListenerModeController(ModeController *modeController) :
    m_modeController(modeController)
{}

bool ActionListenerModeController::handleAction(Action action)
{
    switch (action) {
        case ACTION_POWER_TOGGLE:
            m_modeController->setPower(!m_modeController->getPower());
            return true;

        default:
            return false;
    }
}

}
