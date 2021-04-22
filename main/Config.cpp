#include "Config.h"
#include <ConfigManager.h>
#include <ConfigPropertyConstraint.h>
#include <MatrixOptions.h>

namespace LedWall
{

const int Config::POWER_STATE_FALLBACK = 1;
const int Config::LED_MODE_AUTO_RESTORE_LAST = -1;
const int Config::LED_MODE_FALLBACK = 0;

void Config::init()
{
    matrixOptions()
        ->setVisibility(EBLi::config::ConfigProperty::Device)
        ->setDefaultValue(MatrixOptions::MatrixInvertHorizontal)
        ->setConstraint(EBLi::config::ConfigPropertyConstraint::Number(0, 128));

    matrixWidth()
        ->setVisibility(EBLi::config::ConfigProperty::Device)
        ->setDefaultValue(4)
        ->setConstraint(EBLi::config::ConfigPropertyConstraint::Number(1, 100));

    matrixHeight()
        ->setVisibility(EBLi::config::ConfigProperty::Device)
        ->setDefaultValue(4)
        ->setConstraint(EBLi::config::ConfigPropertyConstraint::Number(1, 100));

    brightness()
        ->setDefaultValue(255)
        ->setConstraint(EBLi::config::ConfigPropertyConstraint::Number(0, 255))
//        ->setChangeHandler([controller](EBLi::ConfigProperty *property){
//            controller->setBrightness(property->getValue<int>()); // FIXME endless loop
//        })
        ;

    powerOnMode()
        ->setVisibility(EBLi::config::ConfigProperty::Device)
        ->setDefaultValue(PowerOnMode::RECOVER_LAST)
        ->setConstraint(EBLi::config::ConfigPropertyConstraint::Number(PowerOnMode::ALWAYS_OFF, PowerOnMode::RECOVER_LAST));

    powerLastState()
        ->setVisibility(EBLi::config::ConfigProperty::Hidden)
        ->setDefaultValue(POWER_STATE_FALLBACK)
        ->setConstraint(EBLi::config::ConfigPropertyConstraint::Number(0, 1))
        ;

    ledModeAutoRestore()
        ->setVisibility(EBLi::config::ConfigProperty::Device)
        // min:-1, max:ledModeCount-1
        ->setDefaultValue(LED_MODE_AUTO_RESTORE_LAST);

    ledModeLast()
        ->setVisibility(EBLi::config::ConfigProperty::Hidden)
        // min:0, max:ledModeCount-1
        ->setDefaultValue(LED_MODE_FALLBACK)
        ;
}

EBLi::config::ConfigProperty * Config::matrixOptions()
{
    return EBLi::config::ConfigManager::instance()->property("matrix_options", "MatrixOptions");
}

EBLi::config::ConfigProperty * Config::matrixWidth()
{
    return EBLi::config::ConfigManager::instance()->property("matrix_width", "MatrixWidth");
}

EBLi::config::ConfigProperty * Config::matrixHeight()
{
    return EBLi::config::ConfigManager::instance()->property("matrix_height", "MatrixHeight");
}

EBLi::config::ConfigProperty * Config::brightness()
{
    return EBLi::config::ConfigManager::instance()->property("matrix_brghtnss", "Brightness");
}

EBLi::config::ConfigProperty * Config::powerOnMode()
{
    return EBLi::config::ConfigManager::instance()->property("pwr_on_mode", "PowerOnResetMode");
}

EBLi::config::ConfigProperty * Config::powerLastState()
{
    return EBLi::config::ConfigManager::instance()->property("pwr_last_state");
}

EBLi::config::ConfigProperty * Config::ledModeAutoRestore()
{
    return EBLi::config::ConfigManager::instance()->property("led_mode_auto", "LedModeAutoRestore");
}

EBLi::config::ConfigProperty * Config::ledModeLast()
{
    return EBLi::config::ConfigManager::instance()->property("led_mode_last");
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
