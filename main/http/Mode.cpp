#include "Mode.h"
#include "../LedMode/LedModes.h"

namespace LedWall::http {

Mode::Mode(ModeController *controller):
    m_controller(controller),
    m_get_mode_uri {
        .uri = BASE_URI "/led/mode",
        .method = http_method::HTTP_GET,
        .handler = Mode::getModeHttpHandler,
        .user_ctx = m_controller
    },
    m_post_mode_uri {
        .uri = BASE_URI "/led/mode",
        .method = http_method::HTTP_POST,
        .handler = Mode::postModeHttpHandler,
        .user_ctx = m_controller
    }
{}

std::vector<httpd_uri_t *> Mode::getHandlers()
{
    return {&m_get_mode_uri, &m_post_mode_uri};
}

esp_err_t Mode::getModeHttpHandler(httpd_req_t *request)
{
    auto controller = static_cast<ModeController*>(request->user_ctx);
    if (nullptr == controller) {
        return ESP_FAIL;
    }

    return sendJsonResponse(createLedModeData(controller), request);
}

esp_err_t Mode::postModeHttpHandler(httpd_req_t *request)
{
    auto controller = static_cast<ModeController*>(request->user_ctx);
    if (nullptr == controller) {
        return ESP_FAIL;
    }

    return jsonRequestHelper(request, [controller](cJSON *jsonRequestData, cJSON **jsonResponseData) -> bool {
        bool modeHasBeenSet = false;

        cJSON *const modeByIndex = cJSON_GetObjectItem(jsonRequestData, "index");
        cJSON *const modeByName = cJSON_GetObjectItem(jsonRequestData, "name");
        if (modeByIndex && cJSON_IsNumber(modeByIndex)) {
            modeHasBeenSet = controller->setModeByIndex(modeByIndex->valueint);
        } else if (modeByName && cJSON_IsString(modeByName) && strlen(modeByName->valuestring) <= ::LedWall::Mode::ValidModeNameLength) {
            modeHasBeenSet = controller->setModeByName(modeByName->valuestring);
        } else {
            return false;
        }

        *jsonResponseData = createLedModeData(controller);
        return modeHasBeenSet;
    });
}

cJSON * Mode::createLedModeData(ModeController *controller)
{
    cJSON *root = cJSON_CreateObject();
    if (nullptr == root) {
        return nullptr;
    }

    cJSON_AddNumberToObject(root, "index", controller->getModeIndex());
    cJSON_AddStringToObject(root, "name", ::LedWall::Mode::LedModes.at(controller->getModeIndex()).name);

    cJSON *options = cJSON_AddObjectToObject(root, "options");
    controller->getModeOptions(options);

    return root;
}

}
