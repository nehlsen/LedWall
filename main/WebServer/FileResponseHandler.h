#ifndef LEDWALL_FILERESPONSEHANDLER_H
#define LEDWALL_FILERESPONSEHANDLER_H

#include <esp_http_server.h>
#include <esp_vfs.h>

#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + 128)

class FileResponseHandler
{
public:
    static esp_err_t handleRequest(httpd_req_t *request);

protected:
    explicit FileResponseHandler(httpd_req_t *request);

    esp_err_t doIt();

    void determineRequestedFile();
    void setMimeType();
    esp_err_t sendFile();

    httpd_req_t *m_request;

    char m_requestedFile[FILE_PATH_MAX];
    int m_fd;
};

#endif //LEDWALL_FILERESPONSEHANDLER_H
