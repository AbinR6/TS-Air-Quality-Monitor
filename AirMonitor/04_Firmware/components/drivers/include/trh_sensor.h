#ifndef TRH_SENSOR_H
#define TRH_SENSOR_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float temperature_c;
    float humidity_rh;
    bool valid;
} trh_data_t;

/**
 * @brief Initialize SHT4x sensor.
 */
esp_err_t trh_sensor_init(void);

/**
 * @brief Trigger high precision T/RH measurement and read converted results.
 * @param[out] data Output data structure.
 */
esp_err_t trh_sensor_read(trh_data_t *data);

#ifdef __cplusplus
}
#endif

#endif /* TRH_SENSOR_H */
