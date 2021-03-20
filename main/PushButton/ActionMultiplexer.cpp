#include "ActionMultiplexer.h"
#include "../ModeController.h"
#include <esp_log.h>

namespace LedWall {

ActionMultiplexer::ActionMultiplexer(ModeController *modeController):
    m_modeController(modeController)
{}

void ActionMultiplexer::triggerAction(Action action)
{
    switch (action) {
        case ACTION_POWER_TOGGLE:
            ESP_LOGI("ActionMultiplexer::triggerAction", "power toggle");
            m_modeController->setPower(!m_modeController->getPower());
            break;

        default:
        case ACTION_NONE:
            ESP_LOGW("ActionMultiplexer::triggerAction", "unknown action");
            break;
    }
}

}
