#include "Preset.h"
#include "../PresetManager/PresetManager.h"

namespace LedWall::http {

Preset::Preset(ModeController *controller):
    m_controller(controller),
    m_get_presets_uri {
        .uri = BASE_URI "/led/presets",
        .method = http_method::HTTP_GET,
        .handler = Preset::getPresetsHttpHandler,
        .user_ctx = nullptr
    },
    m_save_preset_uri {
        .uri = BASE_URI "/led/preset/save",
        .method = http_method::HTTP_POST,
        .handler = Preset::savePresetHttpHandler,
        .user_ctx = m_controller
    },
    m_load_preset_uri {
        .uri = BASE_URI "/led/preset/load",
        .method = http_method::HTTP_POST,
        .handler = Preset::loadPresetHttpHandler,
        .user_ctx = m_controller
    },
    m_delete_preset_uri {
        .uri = BASE_URI "/led/preset/delete",
        .method = http_method::HTTP_POST,
        .handler = Preset::deletePresetHttpHandler,
        .user_ctx = m_controller
    },
    m_clear_presets_uri {
        .uri = BASE_URI "/led/preset/clear",
        .method = http_method::HTTP_POST,
        .handler = Preset::clearPresetsHttpHandler,
        .user_ctx = m_controller
    }
{}

std::vector<httpd_uri_t *> Preset::getHandlers()
{
    return {&m_get_presets_uri, &m_save_preset_uri, &m_load_preset_uri, &m_delete_preset_uri, &m_clear_presets_uri};
}

esp_err_t Preset::getPresetsHttpHandler(httpd_req_t *request)
{
    PresetManager pm;

    cJSON *jsonRoot = cJSON_CreateObject();
    auto presetList = cJSON_AddArrayToObject(jsonRoot, "presets");
    for (const auto& preset : pm.getPresets()) {
        cJSON_AddItemToArray(presetList, cJSON_CreateString(preset.getPresetName().c_str()));
    }

    return sendJsonResponse(jsonRoot, request);
}

esp_err_t Preset::savePresetHttpHandler(httpd_req_t *request)
{
    auto controller = static_cast<ModeController*>(request->user_ctx);
    if (nullptr == controller) {
        return ESP_FAIL;
    }

    return jsonRequestHelper(request, [controller](cJSON *jsonRequestData, cJSON **jsonResponseData) {
        cJSON *const presetName = cJSON_GetObjectItem(jsonRequestData, "name");
        if (cJSON_IsString(presetName) && strlen(presetName->valuestring) <= ::LedWall::Preset::ValidNameLength) {
            controller->savePreset(presetName->valuestring);
        } else {
            return false;
        }

        PresetManager pm;
        if (pm.hasPreset(presetName->valuestring)) {
            *jsonResponseData = createPresetData(pm.getPreset(presetName->valuestring));
        }

        return true;
    });
}

esp_err_t Preset::loadPresetHttpHandler(httpd_req_t *request)
{
    auto controller = static_cast<ModeController*>(request->user_ctx);
    if (nullptr == controller) {
        return ESP_FAIL;
    }

    return jsonRequestHelper(request, [controller](cJSON *jsonRequestData, cJSON **jsonResponseData) {
        cJSON *const presetName = cJSON_GetObjectItem(jsonRequestData, "name");
        if (cJSON_IsString(presetName) && strlen(presetName->valuestring) <= ::LedWall::Preset::ValidNameLength) {
            if (!controller->loadPreset(presetName->valuestring)) {
                return false;
            }
        } else {
            return false;
        }

        PresetManager pm;
        if (pm.hasPreset(presetName->valuestring)) {
            *jsonResponseData = createPresetData(pm.getPreset(presetName->valuestring));
        }

        return true;
    });
}

esp_err_t Preset::deletePresetHttpHandler(httpd_req_t *request)
{
    auto controller = static_cast<ModeController*>(request->user_ctx);
    if (nullptr == controller) {
        return ESP_FAIL;
    }

    return jsonRequestHelper(request, [controller](cJSON *jsonRequestData, cJSON **jsonResponseData) {
        cJSON *const presetName = cJSON_GetObjectItem(jsonRequestData, "name");
        if (cJSON_IsString(presetName) && strlen(presetName->valuestring) <= ::LedWall::Preset::ValidNameLength) {
            controller->deletePreset(presetName->valuestring);
        } else {
            return false;
        }

        return true;
    });
}

esp_err_t Preset::clearPresetsHttpHandler(httpd_req_t *request)
{
    auto controller = static_cast<ModeController*>(request->user_ctx);
    if (nullptr == controller) {
        return ESP_FAIL;
    }

    controller->deleteAllPresets();
    httpd_resp_send(request, "", 0);
    return ESP_OK;
}

cJSON * Preset::createPresetData(const ::LedWall::Preset &preset)
{
    cJSON *presetJson = cJSON_CreateObject();
    cJSON_AddStringToObject(presetJson, "name", preset.getPresetName().c_str());
    cJSON_AddStringToObject(presetJson, "mode", preset.getModeName().c_str());
    cJSON_AddItemToObject(presetJson, "options", cJSON_Duplicate(preset.getModeOptions(), true));

    return presetJson;
}

}
