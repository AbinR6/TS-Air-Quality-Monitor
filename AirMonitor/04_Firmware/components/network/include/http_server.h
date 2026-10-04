#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Start local embedded REST HTTP server (port 80).
 */
esp_err_t http_server_start(void);

/**
 * @brief Stop REST HTTP server.
 */
esp_err_t http_server_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* HTTP_SERVER_H */
