#include "Config.h"
#include <ConfigManager.h>
#include <ConfigPropertyConstraint.h>

namespace LedWall
{

const int Config::POWER_STATE_FALLBACK = 1;
const int Config::LED_MODE_AUTO_RESTORE_LAST = -1;
const int Config::LED_MODE_FALLBACK = 0;

void Config::init()
{
    matrixWidth()
        ->setVisibility(EBLi::ConfigProperty::Device)
        ->setDefaultValue(4)
        ->setConstraint(EBLi::ConfigPropertyConstraint::Number(1, 100));

    matrixHeight()
        ->setVisibility(EBLi::ConfigProperty::Device)
        ->setDefaultValue(4)
        ->setConstraint(EBLi::ConfigPropertyConstraint::Number(1, 100));

    brightness()
        ->setDefaultValue(255)
        ->setConstraint(EBLi::ConfigPropertyConstraint::Number(0, 255))
//        ->setChangeHandler([controller](EBLi::ConfigProperty *property){
//            controller->setBrightness(property->getValue<int>()); // FIXME endless loop
//        })
        ;

    powerOnMode()
        ->setVisibility(EBLi::ConfigProperty::Device)
        ->setDefaultValue(PowerOnMode::RECOVER_LAST)
        ->setConstraint(EBLi::ConfigPropertyConstraint::Number(PowerOnMode::ALWAYS_OFF, PowerOnMode::RECOVER_LAST));

    powerLastState()
        ->setVisibility(EBLi::ConfigProperty::Hidden)
        ->setDefaultValue(POWER_STATE_FALLBACK)
        ->setConstraint(EBLi::ConfigPropertyConstraint::Number(0, 1))
        ;

    ledModeAutoRestore()
        ->setVisibility(EBLi::ConfigProperty::Device)
        // min:-1, max:ledModeCount-1
        ->setDefaultValue(LED_MODE_AUTO_RESTORE_LAST);

    ledModeLast()
        ->setVisibility(EBLi::ConfigProperty::Hidden)
        // min:0, max:ledModeCount-1
        ->setDefaultValue(LED_MODE_FALLBACK)
        ;
}

EBLi::ConfigProperty * Config::matrixWidth()
{
    return EBLi::ConfigManager::instance()->property("matrix_width", "MatrixWidth");
}

EBLi::ConfigProperty * Config::matrixHeight()
{
    return EBLi::ConfigManager::instance()->property("matrix_height", "MatrixHeight");
}

EBLi::ConfigProperty * Config::brightness()
{
    return EBLi::ConfigManager::instance()->property("matrix_brghtnss", "Brightness");
}

EBLi::ConfigProperty * Config::powerOnMode()
{
    return EBLi::ConfigManager::instance()->property("pwr_on_mode", "PowerOnResetMode");
}

EBLi::ConfigProperty * Config::powerLastState()
{
    return EBLi::ConfigManager::instance()->property("pwr_last_state");
}

EBLi::ConfigProperty * Config::ledModeAutoRestore()
{
    return EBLi::ConfigManager::instance()->property("led_mode_auto", "LedModeAutoRestore");
}

EBLi::ConfigProperty * Config::ledModeLast()
{
    return EBLi::ConfigManager::instance()->property("led_mode_last");
}

bool Config::isPoweredOnBoot()
{
    const int mode = powerOnMode()->getValue<int>();
    switch (mode) {
        case ALWAYS_OFF:
            return false;

        case ALWAYS_ON:
            return true;

        default:
        case RECOVER_LAST:
            return powerLastState();
    }
}

int Config::bootIntoLedMode()
{
    const int ledModeAutoRestoreValue = ledModeAutoRestore()->getValue<int>();
    const bool isRecoverLastModeEnabled = ledModeAutoRestoreValue == LED_MODE_AUTO_RESTORE_LAST;

    if (isRecoverLastModeEnabled) {
        return ledModeLast()->getValue<int>();
    }

    return ledModeAutoRestoreValue;
}

}
