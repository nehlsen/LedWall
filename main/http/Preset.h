#ifndef LEDWALL_HTTP_PRESET_H
#define LEDWALL_HTTP_PRESET_H

#include <HttpModule.h>
#include "../ModeController.h"
#include "../PresetManager/Preset.h"

namespace LedWall::http {

class Preset: public EBLi::http::module::HttpModule
{
public:
    explicit Preset(ModeController *controller);

    [[nodiscard]] std::vector<httpd_uri_t *> getHandlers() override;

private:
    ModeController *m_controller;

    httpd_uri_t m_get_presets_uri;
    static esp_err_t getPresetsHttpHandler(httpd_req_t *request);
    httpd_uri_t m_save_preset_uri;
    static esp_err_t savePresetHttpHandler(httpd_req_t *request);
    httpd_uri_t m_load_preset_uri;
    static esp_err_t loadPresetHttpHandler(httpd_req_t *request);
    httpd_uri_t m_delete_preset_uri;
    static esp_err_t deletePresetHttpHandler(httpd_req_t *request);
    httpd_uri_t m_clear_presets_uri;
    static esp_err_t clearPresetsHttpHandler(httpd_req_t *request);

    static cJSON *createPresetData(const ::LedWall::Preset &preset);
};

}

#endif //LEDWALL_HTTP_PRESET_H
