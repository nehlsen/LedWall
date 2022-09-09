#include "PresetChangerModule.h"
#include "../PresetManager/PresetChanger.h"

namespace LedWall::http {

PresetChangerModule::PresetChangerModule(PresetChanger *presetChanger) :
    m_get_options_uri{
        .uri = BASE_URI "/led/presetChanger/options",
        .method = http_method::HTTP_GET,
        .handler = PresetChangerModule::getOptionsHttpHandler,
        .user_ctx = presetChanger
    },
    m_set_options_uri{
        .uri = BASE_URI "/led/presetChanger/options",
        .method = http_method::HTTP_POST,
        .handler = PresetChangerModule::setOptionsHttpHandler,
        .user_ctx = presetChanger
    },
    m_preset_index_uri{
        .uri = BASE_URI "/led/presetChanger/presets",
        .method = http_method::HTTP_GET,
        .handler = PresetChangerModule::getPresetIndexHttpHandler,
        .user_ctx = presetChanger
    },
    m_add_preset_uri{
        .uri = BASE_URI "/led/presetChanger/presets/add",
        .method = http_method::HTTP_POST,
        .handler = PresetChangerModule::addPresetHttpHandler,
        .user_ctx = presetChanger
    },
    m_remove_preset_uri{
        .uri = BASE_URI "/led/presetChanger/presets/remove",
        .method = http_method::HTTP_POST,
        .handler = PresetChangerModule::removePresetHttpHandler,
        .user_ctx = presetChanger
    }
{}

std::vector<httpd_uri_t *> PresetChangerModule::getHandlers()
{
    return {&m_get_options_uri, &m_set_options_uri, &m_preset_index_uri, &m_add_preset_uri, &m_remove_preset_uri};
}

esp_err_t PresetChangerModule::getOptionsHttpHandler(httpd_req_t *request)
{
    auto presetChanger = static_cast<PresetChanger*>(request->user_ctx);
    if (nullptr == presetChanger) {
        return ESP_FAIL;
    }

    return sendJsonResponse(optionsToJson(presetChanger), request);
}

esp_err_t PresetChangerModule::setOptionsHttpHandler(httpd_req_t *request)
{
    auto presetChanger = static_cast<PresetChanger*>(request->user_ctx);
    if (nullptr == presetChanger) {
        return ESP_FAIL;
    }

    return jsonRequestHelper(request, [presetChanger](cJSON *jsonRequestData, cJSON **jsonResponseData) {
        cJSON *const isRandom = cJSON_GetObjectItem(jsonRequestData, "random");
        if (cJSON_IsBool(isRandom)) {
            presetChanger->setRandomEnabled(cJSON_IsTrue(isRandom));
        }
        cJSON *const delay = cJSON_GetObjectItem(jsonRequestData, "delay");
        if (cJSON_IsNumber(delay)) {
            presetChanger->setAutoChangeDelay(delay->valueint);
        }
        cJSON *const isAuto = cJSON_GetObjectItem(jsonRequestData, "auto");
        if (cJSON_IsBool(isAuto)) {
            presetChanger->setAutoChangeEnabled(cJSON_IsTrue(isAuto));
        }

        *jsonResponseData = optionsToJson(presetChanger);
        return true;
    });
}

esp_err_t PresetChangerModule::getPresetIndexHttpHandler(httpd_req_t *request)
{
    auto presetChanger = static_cast<PresetChanger*>(request->user_ctx);
    if (nullptr == presetChanger) {
        return ESP_FAIL;
    }

    cJSON *jsonRoot = cJSON_CreateObject();
    auto presetList = cJSON_AddArrayToObject(jsonRoot, "presets");
    for (const auto& presetName : presetChanger->getPresets()) {
        cJSON_AddItemToArray(presetList, cJSON_CreateString(presetName.c_str()));
    }

    return sendJsonResponse(jsonRoot, request);
}

esp_err_t PresetChangerModule::addPresetHttpHandler(httpd_req_t *request)
{
    // TODO implement add preset
    return ESP_FAIL;
}

esp_err_t PresetChangerModule::removePresetHttpHandler(httpd_req_t *request)
{
    // TODO implement remove preset
    return ESP_FAIL;
}

cJSON *PresetChangerModule::optionsToJson(PresetChanger *presetChanger)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddBoolToObject(root, "random", presetChanger->isRandomEnabled());
    cJSON_AddBoolToObject(root, "auto", presetChanger->isAutoChangeEnabled());
    cJSON_AddNumberToObject(root, "delay", presetChanger->getAutoChangeDelay());

    return root;
}

}
