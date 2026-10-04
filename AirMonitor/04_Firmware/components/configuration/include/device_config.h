#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char device_id[32];
    char wifi_ssid[32];
    char wifi_password[64];
    char mqtt_broker_uri[64];
    uint16_t reporting_interval_s;
    uint8_t display_brightness_pct;
    bool simulation_mode;
} runtime_config_t;

/**
 * @brief Load runtime configuration from NVS, falling back to defaults if unprogrammed.
 */
esp_err_t device_config_load(runtime_config_t *config);

/**
 * @brief Save runtime configuration changes back to NVS.
 */
esp_err_t device_config_save(const runtime_config_t *config);

#ifdef __cplusplus
}
#endif

#endif /* DEVICE_CONFIG_H */
