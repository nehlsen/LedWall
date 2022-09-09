#ifndef LEDWALL_PRESETCHANGERMODULE_H
#define LEDWALL_PRESETCHANGERMODULE_H

#include <HttpModule.h>

namespace LedWall {
class PresetChanger;
namespace http {

class PresetChangerModule: public EBLi::http::module::HttpModule
{
public:
    explicit PresetChangerModule(PresetChanger *presetChanger);

    [[nodiscard]] std::vector<httpd_uri_t *> getHandlers() override;

private:
    httpd_uri_t m_get_options_uri;
    static esp_err_t getOptionsHttpHandler(httpd_req_t *request);

    httpd_uri_t m_set_options_uri;
    static esp_err_t setOptionsHttpHandler(httpd_req_t *request);

    httpd_uri_t m_preset_index_uri;
    static esp_err_t getPresetIndexHttpHandler(httpd_req_t *request);

    httpd_uri_t m_add_preset_uri;
    static esp_err_t addPresetHttpHandler(httpd_req_t *request);

    httpd_uri_t m_remove_preset_uri;
    static esp_err_t removePresetHttpHandler(httpd_req_t *request);

    static cJSON *optionsToJson(PresetChanger *presetChanger);
};

}
}

#endif //LEDWALL_PRESETCHANGERMODULE_H
