#ifndef SYSTEM_INIT_H
#define SYSTEM_INIT_H

#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool nvs_ok;
    bool psram_ok;
    bool i2c_bus_ok;
    bool pm_sensor_ok;
    bool co2_sensor_ok;
    bool trh_sensor_ok;
    bool display_ok;
    bool touch_ok;
    bool wifi_ok;
} system_health_status_t;

/**
 * @brief Initialize low-level peripherals, clocks, memory, and buses.
 * @return ESP_OK on success, or error code.
 */
esp_err_t system_hardware_init(void);

/**
 * @brief Execute Power-On Self Test (POST) and populate health metrics.
 * @param[out] status Pointer to health status structure.
 * @return ESP_OK if minimum boot prerequisites pass.
 */
esp_err_t system_run_post(system_health_status_t *status);

/**
 * @brief Print boot diagnostic banner to serial console.
 */
void system_print_banner(const system_health_status_t *status);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_INIT_H */
