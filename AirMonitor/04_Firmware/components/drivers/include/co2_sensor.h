#ifndef CO2_SENSOR_H
#define CO2_SENSOR_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint16_t co2_ppm;
    float temperature_c;
    float humidity_rh;
    bool valid;
} co2_data_t;

/**
 * @brief Initialize the SCD4x I2C interface and send start periodic measurement command.
 */
esp_err_t co2_sensor_init(void);

/**
 * @brief Read periodic measurement from SCD4x sensor (CO2, Temp, RH).
 * @param[out] data Output data structure.
 * @return ESP_OK on success, or error code.
 */
esp_err_t co2_sensor_read(co2_data_t *data);

/**
 * @brief Stop periodic measurement mode.
 */
esp_err_t co2_sensor_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* CO2_SENSOR_H */
