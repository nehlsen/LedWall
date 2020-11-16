#include "ModeOptions.h"
#include "../LedMode/LedModes.h"

namespace LedWall::http {

ModeOptions::ModeOptions(ModeController *controller) : m_controller(controller)
{}

std::vector<EBLi::http::module::HttpModule::HttpEndpoint> ModeOptions::getHttpEndpoints() const
{
    auto createLedModeData = [=]() {
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "index", m_controller->getModeIndex());
        cJSON_AddStringToObject(root, "name", Mode::LedModes.at(m_controller->getModeIndex()).name);

        cJSON *options = cJSON_AddObjectToObject(root, "options");
        m_controller->getModeOptions(options);

        return root;
    };

//    auto getModeOptionsHandler = [](httpd_req_t *request) {
//    };

    auto postModeOptionsHandler = [=](httpd_req_t *request) {
        return jsonRequestHelper(request, [=](cJSON *jsonRequestData, cJSON **jsonResponseData) -> bool {
            bool optionsHaveBeenSet = m_controller->setModeOptions(jsonRequestData);
            *jsonResponseData = createLedModeData();
            return optionsHaveBeenSet;
        });
    };

    return {
//        HttpEndpoint {
//            .method = HTTP_GET,
//            .uri = "/led/mode/options",
//            .handler = getModeOptionsHandler,
//        },
        HttpEndpoint {
            .method = HTTP_POST,
            .uri = "/led/mode/options",
            .handler = postModeOptionsHandler,
        },
        // TODO delete mode options
//        HttpEndpoint {
//            .method = HTTP_DELETE,
//            .uri = "/led/mode/options",
//            .handler = deleteModeOptionsHandler,
//        },
    };
}

}
