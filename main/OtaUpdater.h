#ifndef LEDWALL_OTAUPDATER_H
#define LEDWALL_OTAUPDATER_H

#include <esp_err.h>
#include <esp_https_ota.h>

class OtaUpdaterPrivate;

class OtaUpdater
{
public:
    OtaUpdater();

    void setUpdateUrl(const char* url);
    bool startUpdate();

    esp_err_t getLastError() const;

protected:
    OtaUpdaterPrivate *m_updater;
};

#endif //LEDWALL_OTAUPDATER_H
