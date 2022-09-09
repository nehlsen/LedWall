#include "PresetChanger.h"
#include "../ModeController.h"
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <algorithm>
#include <random>

#include "PresetManager.h" // TODO remove once ctor is changed
#include "../events.h"

namespace LedWall {

static const char *LOG_TAG = "PresetChanger";

#define AUTO_CHANGER_ENABLED_BIT BIT2
static EventGroupHandle_t auto_changer_event_group;

[[noreturn]] void timed_auto_change_task(void *pvParameter)
{
    auto *changer = static_cast<PresetChanger*>(pvParameter);
    ESP_ERROR_CHECK(nullptr == changer ? ESP_FAIL : ESP_OK);

    auto now = [] () -> int32_t {
        return esp_timer_get_time() / (1000 * 1000);
    };

    auto checkBit = [] (bool waitForIt = true) -> bool {
        EventBits_t uxBits = xEventGroupWaitBits(auto_changer_event_group, AUTO_CHANGER_ENABLED_BIT, false, false, waitForIt ? portMAX_DELAY : 1);
        if ((uxBits&AUTO_CHANGER_ENABLED_BIT) != AUTO_CHANGER_ENABLED_BIT) {
            return false;
        }
        return true;
    };

    // seconds
    static int32_t lastChangeTimestamp = now();

    do {
        if (!checkBit()) {
            continue;
        }

        const int32_t nextChangeIn = lastChangeTimestamp + changer->getAutoChangeDelay() - now();
        ESP_LOGI(LOG_TAG, "Next change in %ds", nextChangeIn);
        if (nextChangeIn >= 1) {
            const TickType_t xDelay = nextChangeIn * 1000 / portTICK_PERIOD_MS;
            vTaskDelay(xDelay);
        }

        // check if the enabled bit has changed while we were waiting on the delay
        if (!checkBit(false)) {
            continue;
        }

        changer->activateNextPreset();
        lastChangeTimestamp = now();
    } while (true);
}

void preset_changer_led_wall_event(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    auto presetChanger = static_cast<PresetChanger*>(event_handler_arg);
    ESP_ERROR_CHECK(nullptr == presetChanger ? ESP_FAIL : ESP_OK);

    switch (event_id) {
        case LEDWALL_BTN_EVENT_PRESET_NEXT:
            ESP_LOGI(LOG_TAG, "preset_changer_led_wall_event: PRESET_NEXT");
            presetChanger->activateNextPreset();
            break;

        case LEDWALL_BTN_EVENT_TOGGLE_PRESET_AUTO_CHANGE:
            ESP_LOGI(LOG_TAG, "preset_changer_led_wall_event: TOGGLE_PRESET_AUTO_CHANGE");
            presetChanger->setAutoChangeEnabled(!presetChanger->isAutoChangeEnabled());
            break;

        default:
            // NO OP
            break;
    }
}

PresetChanger::PresetChanger(ModeController *modeController) : m_modeController(modeController)
{
    // TODO load and save selected presets
    //   just load all available presets for now
    //   no presets selected? use all!

    PresetManager pm;
    for (const auto& preset : pm.getPresets()) {
        addPreset(preset.getPresetName());
    }
    ESP_LOGI(LOG_TAG, "Starting with %d presets...", m_presets.size());

    m_currentPreset = m_presets.cbegin();

    esp_event_handler_register(LEDWALL_EVENTS, ESP_EVENT_ANY_ID, preset_changer_led_wall_event, this);

    auto_changer_event_group = xEventGroupCreate();
    xTaskCreate(
            &timed_auto_change_task,
            "timed_auto_change_task",
            2 * 1024,
            this,
            tskIDLE_PRIORITY,
            nullptr
    );
}

void PresetChanger::addPreset(const std::string &presetName)
{
    m_presets.push_back(presetName);
}

void PresetChanger::removePreset(const std::string &presetName)
{
    // FIXME this removes only the first occurrence
    auto it = std::find(m_presets.begin(), m_presets.end(), presetName);
    if (it != m_presets.end()) {
        m_presets.erase(it);
    }
}

void PresetChanger::clearPresets()
{
    m_presets.clear();
}

PresetNameList PresetChanger::getPresets() const
{
    return m_presets;
}

void PresetChanger::setRandomEnabled(bool enabled)
{
    ESP_LOGI(LOG_TAG, "setRandomEnabled: %s", enabled ? "random" : "in-sequence");

    m_isRandomEnabled = enabled;
}

bool PresetChanger::isRandomEnabled() const
{
    return m_isRandomEnabled;
}

void PresetChanger::activateNextPreset()
{
    ESP_LOGI(LOG_TAG, "activateNextPreset: %s", isRandomEnabled() ? "random" : "in-sequence");

    if (m_presets.empty()) {
        ESP_LOGW(LOG_TAG, "activateNextPreset: can not activate next preset: no presets configured");
        return;
    }

    if (isRandomEnabled()) {
        activateNextPresetRandom();
    } else {
        activateNextPresetInSequence();
    }
}

void PresetChanger::setAutoChangeDelay(uint16_t seconds)
{
    ESP_LOGI(LOG_TAG, "setAutoChangeDelay: %ds", seconds);

    m_autoChangeDelay = seconds;
}

uint16_t PresetChanger::getAutoChangeDelay() const
{
    return m_autoChangeDelay;
}

void PresetChanger::setAutoChangeEnabled(bool enabled)
{
    ESP_LOGI(LOG_TAG, "setAutoChangeEnabled: %s", enabled ? "enabled" : "disabled");

    m_isAutoChangeEnabled = enabled;
    // TODO change activation?!

    if (enabled) {
        xEventGroupSetBits(auto_changer_event_group, AUTO_CHANGER_ENABLED_BIT);
    } else {
        xEventGroupClearBits(auto_changer_event_group, AUTO_CHANGER_ENABLED_BIT);
    }
}

bool PresetChanger::isAutoChangeEnabled() const
{
    return m_isAutoChangeEnabled;
}

void PresetChanger::activateNextPresetInSequence()
{
    if (m_presets.empty()) {
        ESP_LOGE(LOG_TAG, "activateNextPresetInSequence: can not select next preset, no presets configured");
        return;
    }

    ++m_currentPreset;
    if (m_currentPreset == m_presets.cend()) {
        m_currentPreset = m_presets.cbegin();
    }

    m_modeController->loadPreset(*m_currentPreset);
}

void PresetChanger::activateNextPresetRandom()
{
    if (m_presets.empty()) {
        ESP_LOGE(LOG_TAG, "activateNextPresetRandom: can not select random preset, no presets configured");
        return;
    }
    if (m_presets.size() == 1) {
        ESP_LOGE(LOG_TAG, "activateNextPresetRandom: can not select random preset, only one preset configured");
        m_modeController->loadPreset(*m_presets.cbegin());
    }

    PresetNameList randomPresets;
    const size_t numberOfRandomPresets = 1;
    std::sample(
            m_presets.begin(),
            m_presets.end(),
            std::back_inserter(randomPresets),
            numberOfRandomPresets,
            std::mt19937{std::random_device{}()}
    );

    if (randomPresets.size() != 1) {
        ESP_LOGW(LOG_TAG, "activateNextPresetRandom: failed to select one random preset");
        return;
    }

    m_modeController->loadPreset(*randomPresets.cbegin());
}

}
