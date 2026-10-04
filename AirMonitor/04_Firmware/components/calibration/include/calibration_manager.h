#ifndef CALIBRATION_MANAGER_H
#define CALIBRATION_MANAGER_H

#include <stdint.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float pm25_slope;
    float pm25_offset;
    float co2_slope;
    float co2_offset;
    float temp_offset;
    float rh_offset;
} sensor_calibration_t;

/**
 * @brief Initialize calibration system and load factory/user coefficients from NVS.
 */
esp_err_t calibration_manager_init(void);

/**
 * @brief Apply calibration polynomial to raw PM2.5 reading.
 */
float calibration_apply_pm25(float raw_pm25);

/**
 * @brief Apply zero/span calibration to raw CO2 reading.
 */
float calibration_apply_co2(float raw_co2);

/**
 * @brief Apply temperature offset correction.
 */
float calibration_apply_temp(float raw_temp);

/**
 * @brief Apply humidity offset correction.
 */
float calibration_apply_rh(float raw_rh);

#ifdef __cplusplus
}
#endif

#endif /* CALIBRATION_MANAGER_H */
