#include "Power.h"

namespace LedWall::http {

Power::Power(ModeController *controller):
    m_controller(controller),
    m_get_power_uri {
        .uri = BASE_URI "/led/power",
        .method = http_method::HTTP_GET,
        .handler = Power::getPowerHttpHandler,
        .user_ctx = m_controller
    },
    m_post_power_uri {
        .uri = BASE_URI "/led/power",
        .method = http_method::HTTP_POST,
        .handler = Power::postPowerHttpHandler,
        .user_ctx = m_controller
    }
{}

std::vector<httpd_uri_t *> Power::getHandlers()
{
    return {&m_get_power_uri, &m_post_power_uri};
}

esp_err_t Power::getPowerHttpHandler(httpd_req_t *request)
{
    auto controller = static_cast<ModeController*>(request->user_ctx);
    if (nullptr == controller) {
        return ESP_FAIL;
    }

    return sendJsonResponse(createLedPowerInfo(controller), request);
}

esp_err_t Power::postPowerHttpHandler(httpd_req_t *request)
{
    auto controller = static_cast<ModeController*>(request->user_ctx);
    if (nullptr == controller) {
        return ESP_FAIL;
    }

    return jsonRequestHelper(request, [controller](cJSON *jsonRequestData, cJSON **jsonResponseData) -> bool {
        cJSON *const powerObj = cJSON_GetObjectItem(jsonRequestData, "power");
        if (!powerObj) {
            return false;
        }

        int power = powerObj->valueint;
        controller->setPower(power);
        *jsonResponseData = createLedPowerInfo(controller);
        return true;
    });
}

cJSON * Power::createLedPowerInfo(ModeController *controller)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "power", controller->getPower());

    return root;
}

}
