#include "ModeList.h"
#include "../LedMode/LedModes.h"

namespace LedWall::http {

std::vector<EBLi::http::module::HttpModule::HttpEndpoint> ModeList::getHttpEndpoints() const
{
    auto getModeListHandler = [](httpd_req_t *request) {
        cJSON *root = cJSON_CreateObject();
        cJSON *modeList = cJSON_AddArrayToObject(root, "modes");
        for (Mode::t_LedModes_size idx = 0; idx < Mode::LedModes.size(); ++idx) {
            cJSON *modeDescription = cJSON_CreateObject();
            cJSON_AddNumberToObject(modeDescription, "index", idx);
            cJSON_AddStringToObject(modeDescription, "name", Mode::LedModes.at(idx).name);
            cJSON_AddItemToArray(modeList, modeDescription);
        }

        return sendJsonResponse(root, request);
    };

    return {
        HttpEndpoint {
            .method = HTTP_GET,
            .uri = "/led/modes",
            .handler = getModeListHandler,
        },
    };
}

}
