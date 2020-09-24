#include "Config.h"
#include <ConfigManager.h>
#include <ConfigPropertyConstraintNumber.h>
#include "ModeController.h"

//#define MATRIX_WIDTH_KEY        "matrix_width"
//#define MATRIX_HEIGHT_KEY       "matrix_height"
//#define MATRIX_BRIGHTNESS_KEY   "matrix_brghtnss"
//#define RESTART_COUNTER_KEY     "restart_counter"
//#define POWER_LAST_STATE_KEY    "power_state"
//#define POWER_BOOT_MODE_KEY     "power_boot_into"
//#define LED_MODE_LAST_STATE_KEY "led_mode_state"
//#define LED_MODE_BOOT_MODE_KEY  "led_mode_boot"

namespace LedWall
{

void Config::init(ModeController *controller)
{
    assert(controller != nullptr);

    matrixWidth()
        ->setDefaultValue(4)
        ->setConstraint(new EBLi::ConfigPropertyConstraintNumber(1, 100));

    matrixHeight()
        ->setDefaultValue(4)
        ->setConstraint(new EBLi::ConfigPropertyConstraintNumber(1, 100));

    brightness()
        ->setDefaultValue(255)
        ->setConstraint(new EBLi::ConfigPropertyConstraintNumber(0, 255))
        ->setChangeHandler([controller](EBLi::ConfigProperty *property){
            controller->setBrightness(property->getValue<int>());
        });
}

EBLi::ConfigProperty * Config::matrixWidth()
{
    return EBLi::ConfigManager::instance()->property("matrix_width");
}

EBLi::ConfigProperty * Config::matrixHeight()
{}

EBLi::ConfigProperty * Config::brightness()
{}

}
