#include "Preset.h"
#include "../PresetManager/Preset.h"
#include "../PresetManager/PresetManager.h"

namespace LedWall::http {

Preset::Preset(ModeController *controller) : m_controller(controller)
{}

std::vector<EBLi::http::module::HttpModule::HttpEndpoint> Preset::getHttpEndpoints() const
{
    auto getPresetList = [](httpd_req_t *request) {
        PresetManager pm;

        cJSON *jsonRoot = cJSON_CreateObject();
        auto presetList = cJSON_AddArrayToObject(jsonRoot, "presets");
        for (const auto& preset : pm.getPresets()) {
            cJSON_AddItemToArray(presetList, cJSON_CreateString(preset.getPresetName().c_str()));
        }

        return sendJsonResponse(jsonRoot, request);
    };

    auto createPresetData = [=](const ::LedWall::Preset &preset) {
        cJSON *presetJson = cJSON_CreateObject();
        cJSON_AddStringToObject(presetJson, "name", preset.getPresetName().c_str());
        cJSON_AddStringToObject(presetJson, "mode", preset.getModeName().c_str());
        cJSON_AddItemToObject(presetJson, "options", cJSON_Duplicate(preset.getModeOptions(), true));

        return presetJson;
    };

    auto savePreset = [=](httpd_req_t *request) {
        return jsonRequestHelper(request, [=](cJSON *jsonRequestData, cJSON **jsonResponseData) {
            cJSON *const presetName = cJSON_GetObjectItem(jsonRequestData, "name");
            if (cJSON_IsString(presetName) && strlen(presetName->valuestring) <= ::LedWall::Preset::ValidNameLength) {
                m_controller->savePreset(presetName->valuestring);
            } else {
                return false;
            }

            PresetManager pm;
            if (pm.hasPreset(presetName->valuestring)) {
                *jsonResponseData = createPresetData(pm.getPreset(presetName->valuestring));
            }

            return true;
        });
    };

    auto loadPreset = [=](httpd_req_t *request) {
        return jsonRequestHelper(request, [=](cJSON *jsonRequestData, cJSON **jsonResponseData) {
            cJSON *const presetName = cJSON_GetObjectItem(jsonRequestData, "name");
            if (cJSON_IsString(presetName) && strlen(presetName->valuestring) <= ::LedWall::Preset::ValidNameLength) {
                if (!m_controller->loadPreset(presetName->valuestring)) {
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
    };

    auto deletePreset = [=](httpd_req_t *request) {
        return jsonRequestHelper(request, [=](cJSON *jsonRequestData, cJSON **jsonResponseData) {
            cJSON *const presetName = cJSON_GetObjectItem(jsonRequestData, "name");
            if (cJSON_IsString(presetName) && strlen(presetName->valuestring) <= ::LedWall::Preset::ValidNameLength) {
                LedWall::ModeController::deletePreset(presetName->valuestring);
            } else {
                return false;
            }

            return true;
        });
    };

    auto deleteAllPresets = [=](httpd_req_t *request) {
        LedWall::ModeController::deleteAllPresets();
        return ESP_OK;
    };

    return {
        HttpEndpoint {
            .method = HTTP_GET,
            .uri = "/led/presets",
            .handler = getPresetList,
        },
        HttpEndpoint {
            .method = HTTP_POST,
            .uri = "/led/preset/save",
            .handler = savePreset,
        },
        HttpEndpoint {
            .method = HTTP_POST,
            .uri = "/led/preset/load",
            .handler = loadPreset,
        },
        HttpEndpoint {
            .method = HTTP_POST,
            .uri = "/led/preset/delete",
            .handler = deletePreset,
        },
        HttpEndpoint {
            .method = HTTP_POST,
            .uri = "/led/preset/clear",
            .handler = deleteAllPresets,
        },
    };
}

}
