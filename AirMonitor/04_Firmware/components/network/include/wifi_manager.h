#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    WIFI_STATE_DISCONNECTED = 0,
    WIFI_STATE_CONNECTING,
    WIFI_STATE_CONNECTED,
    WIFI_STATE_RECONNECTING,
} wifi_state_t;

/**
 * @brief Initialize Wi-Fi subsystem in Station mode.
 */
esp_err_t wifi_manager_init(void);

/**
 * @brief Connect to configured Wi-Fi AP.
 */
esp_err_t wifi_manager_connect(const char *ssid, const char *password);

/**
 * @brief Return true if Wi-Fi has acquired an IP address.
 */
bool wifi_manager_is_connected(void);

/**
 * @brief Get current Wi-Fi state.
 */
wifi_state_t wifi_manager_get_state(void);

/**
 * @brief Get current Wi-Fi RSSI signal strength (dBm).
 */
int8_t wifi_manager_get_rssi(void);

#ifdef __cplusplus
}
#endif

#endif /* WIFI_MANAGER_H */
