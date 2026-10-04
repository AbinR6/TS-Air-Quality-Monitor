#ifndef SELF_TEST_H
#define SELF_TEST_H

#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t free_heap_internal;
    uint32_t free_heap_psram;
    uint32_t min_free_heap;
    bool i2c_bus_healthy;
    bool pm_sensor_healthy;
    bool co2_sensor_healthy;
    bool trh_sensor_healthy;
} diagnostics_summary_t;

/**
 * @brief Execute complete diagnostic inspection of system memory and peripherals.
 */
esp_err_t self_test_run(diagnostics_summary_t *diag);

#ifdef __cplusplus
}
#endif

#endif /* SELF_TEST_H */
