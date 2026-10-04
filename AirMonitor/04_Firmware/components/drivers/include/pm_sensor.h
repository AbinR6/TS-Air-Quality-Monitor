#ifndef PM_SENSOR_H
#define PM_SENSOR_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint16_t pm1_0_standard;   /* PM1.0 concentration standard particle [ug/m3] */
    uint16_t pm2_5_standard;   /* PM2.5 concentration standard particle [ug/m3] */
    uint16_t pm10_standard;    /* PM10 concentration standard particle [ug/m3] */
    uint16_t pm1_0_atmospheric;/* PM1.0 concentration under atmospheric environment [ug/m3] */
    uint16_t pm2_5_atmospheric;/* PM2.5 concentration under atmospheric environment [ug/m3] */
    uint16_t pm10_atmospheric; /* PM10 concentration under atmospheric environment [ug/m3] */
    uint16_t particles_0_3um;  /* Particles > 0.3um in 0.1L air */
    uint16_t particles_0_5um;  /* Particles > 0.5um in 0.1L air */
    uint16_t particles_1_0um;  /* Particles > 1.0um in 0.1L air */
    uint16_t particles_2_5um;  /* Particles > 2.5um in 0.1L air */
    uint16_t particles_5_0um;  /* Particles > 5.0um in 0.1L air */
    uint16_t particles_10um;   /* Particles > 10um in 0.1L air */
    bool valid;
} pm_data_t;

/**
 * @brief Initialize the particulate matter UART interface and control pins.
 */
esp_err_t pm_sensor_init(void);

/**
 * @brief Put PM sensor into low-power sleep mode (fan and laser disabled).
 */
esp_err_t pm_sensor_sleep(void);

/**
 * @brief Wake PM sensor up from sleep mode.
 */
esp_err_t pm_sensor_wake(void);

/**
 * @brief Poll for incoming 32-byte data packet and validate checksum.
 * @param[out] data Decoded sensor data struct.
 * @return ESP_OK if valid packet received, ESP_ERR_TIMEOUT or ESP_ERR_INVALID_CRC otherwise.
 */
esp_err_t pm_sensor_read(pm_data_t *data);

#ifdef __cplusplus
}
#endif

#endif /* PM_SENSOR_H */
