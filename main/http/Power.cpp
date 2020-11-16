#include "Power.h"

namespace LedWall::http {

Power::Power(ModeController *controller) : m_controller(controller)
{}

std::vector<EBLi::http::module::HttpModule::HttpEndpoint> Power::getHttpEndpoints() const
{
    auto createLedPowerData = [=]() {
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "power", m_controller->getPower());

        return root;
    };

    auto getPowerHandler = [=](httpd_req_t *request) {
        return sendJsonResponse(createLedPowerData(), request);
    };

    auto postPowerHandler = [=](httpd_req_t *request) {
        return jsonRequestHelper(request, [=](cJSON *jsonRequestData, cJSON **jsonResponseData) -> bool {
            cJSON *const powerObj = cJSON_GetObjectItem(jsonRequestData, "power");
            if (!powerObj) {
                return false;
            }

            int power = powerObj->valueint;
            m_controller->setPower(power);
            *jsonResponseData = createLedPowerData();
            return true;
        });
    };

    return {
        HttpEndpoint {
            .method = HTTP_GET,
            .uri = "/led/power",
            .handler = getPowerHandler,
        },
        HttpEndpoint {
            .method = HTTP_POST,
            .uri = "/led/power",
            .handler = postPowerHandler,
        },
    };
}

}
