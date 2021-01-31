#include "ModeOptions.h"
#include "../LedMode/LedModes.h"

namespace LedWall::http {

ModeOptions::ModeOptions(ModeController *controller):
    m_controller(controller),
    m_get_mode_options_uri {
        .uri = BASE_URI "/led/mode/options",
        .method = http_method::HTTP_GET,
        .handler = ModeOptions::getModeOptionsHttpHandler,
        .user_ctx = m_controller
    },
    m_post_mode_options_uri {
        .uri = BASE_URI "/led/mode/options",
        .method = http_method::HTTP_POST,
        .handler = ModeOptions::postModeOptionsHttpHandler,
        .user_ctx = m_controller
    },
    m_delete_mode_options_uri {
        .uri = BASE_URI "/led/mode/options",
        .method = http_method::HTTP_DELETE,
        .handler = ModeOptions::deleteModeOptionsHttpHandler,
        .user_ctx = m_controller
    }
{}

std::vector<httpd_uri_t *> ModeOptions::getHandlers()
{
    return {&m_get_mode_options_uri, &m_post_mode_options_uri, &m_delete_mode_options_uri};
}

esp_err_t ModeOptions::getModeOptionsHttpHandler(httpd_req_t *request)
{
    auto controller = static_cast<ModeController*>(request->user_ctx);
    if (nullptr == controller) {
        return ESP_FAIL;
    }

    cJSON *options = cJSON_CreateObject();
    controller->getModeOptions(options);

    return sendJsonResponse(options, request);
}

esp_err_t ModeOptions::postModeOptionsHttpHandler(httpd_req_t *request)
{
    auto controller = static_cast<ModeController*>(request->user_ctx);
    if (nullptr == controller) {
        return ESP_FAIL;
    }

    return jsonRequestHelper(request, [controller](cJSON *jsonRequestData, cJSON **jsonResponseData) -> bool {
        bool optionsHaveBeenSet = controller->setModeOptions(jsonRequestData);

        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "index", controller->getModeIndex());
        cJSON_AddStringToObject(root, "name", Mode::LedModes.at(controller->getModeIndex()).name);

        cJSON *options = cJSON_AddObjectToObject(root, "options");
        controller->getModeOptions(options);

        *jsonResponseData = root;
        return optionsHaveBeenSet;
    });
}

esp_err_t ModeOptions::deleteModeOptionsHttpHandler(httpd_req_t *request)
{
    auto controller = static_cast<ModeController*>(request->user_ctx);
    if (nullptr == controller) {
        return ESP_FAIL;
    }

    controller->resetModeOptions();
    httpd_resp_send(request, "", 0);
    return ESP_OK;
}

}
