#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float battery_voltage;
    uint8_t battery_percent;
    bool is_charging;
    bool is_usb_connected;
} power_status_t;

/**
 * @brief Initialize battery ADC channel and charging GPIO.
 */
esp_err_t power_manager_init(void);

/**
 * @brief Sample battery voltage and compute percentage state of charge.
 */
esp_err_t power_manager_get_status(power_status_t *status);

#ifdef __cplusplus
}
#endif

#endif /* POWER_MANAGER_H */
