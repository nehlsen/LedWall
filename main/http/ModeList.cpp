#include "ModeList.h"
#include "../LedMode/LedModes.h"

namespace LedWall::http {

ModeList::ModeList():
    m_get_mode_list_uri {
        .uri = BASE_URI "/led/modes",
        .method = http_method::HTTP_GET,
        .handler = ModeList::getModeListHttpHandler,
        .user_ctx = nullptr
    }
{}

std::vector<httpd_uri_t *> ModeList::getHandlers()
{
    return {&m_get_mode_list_uri};
}

esp_err_t ModeList::getModeListHttpHandler(httpd_req_t *request)
{
    cJSON *root = cJSON_CreateObject();
    cJSON *modeList = cJSON_AddArrayToObject(root, "modes");
    for (Mode::t_LedModes_size idx = 0; idx < Mode::LedModes.size(); ++idx) {
        cJSON *modeDescription = cJSON_CreateObject();
        cJSON_AddNumberToObject(modeDescription, "index", idx);
        cJSON_AddStringToObject(modeDescription, "name", Mode::LedModes.at(idx).name);
        cJSON_AddItemToArray(modeList, modeDescription);
    }

    return sendJsonResponse(root, request);
}

}
