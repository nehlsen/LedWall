#include "Mode.h"
#include "../LedMode/LedModes.h"

namespace LedWall::http {

Mode::Mode(ModeController *controller) : m_controller(controller)
{}

std::vector<EBLi::http::module::HttpModule::HttpEndpoint> Mode::getHttpEndpoints() const
{
    auto createLedModeData = [=]() {
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "index", m_controller->getModeIndex());
        cJSON_AddStringToObject(root, "name", ::LedWall::Mode::LedModes.at(m_controller->getModeIndex()).name);

        cJSON *options = cJSON_AddObjectToObject(root, "options");
        m_controller->getModeOptions(options);

        return root;
    };

    auto getModeHandler = [=](httpd_req_t *request) {
        return sendJsonResponse(createLedModeData(), request);
    };

    auto postModeHandler = [=](httpd_req_t *request) {
        return jsonRequestHelper(request, [=](cJSON *jsonRequestData, cJSON **jsonResponseData) -> bool {
            bool modeHasBeenSet = false;

            cJSON *const modeByIndex = cJSON_GetObjectItem(jsonRequestData, "index");
            cJSON *const modeByName = cJSON_GetObjectItem(jsonRequestData, "name");
            if (modeByIndex && cJSON_IsNumber(modeByIndex)) {
                modeHasBeenSet = m_controller->setModeByIndex(modeByIndex->valueint);
            } else if (modeByName && cJSON_IsString(modeByName) && strlen(modeByName->valuestring) <= ::LedWall::Mode::ValidModeNameLength) {
                modeHasBeenSet = m_controller->setModeByName(modeByName->valuestring);
            } else {
                return false;
            }

            *jsonResponseData = createLedModeData();
            return modeHasBeenSet;
        });
    };

    return {
        HttpEndpoint {
            .method = HTTP_GET,
            .uri = "/led/mode",
            .handler = getModeHandler,
        },
        HttpEndpoint {
            .method = HTTP_POST,
            .uri = "/led/mode",
            .handler = postModeHandler,
        },
    };
}

}
