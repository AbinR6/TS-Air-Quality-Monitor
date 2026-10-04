#ifndef MEASUREMENT_PIPELINE_H
#define MEASUREMENT_PIPELINE_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t timestamp;
    float pm1_0;
    float pm2_5;
    float pm10;
    float co2_ppm;
    float temperature_c;
    float humidity_rh;
    uint16_t aqi;
    bool pm_valid;
    bool co2_valid;
    bool trh_valid;
} processed_metrics_t;

/**
 * @brief Initialize the measurement processing pipeline, buffers, and filters.
 */
esp_err_t measurement_pipeline_init(void);

/**
 * @brief Ingest raw sensor measurements and compute smoothed/filtered metrics.
 */
esp_err_t measurement_pipeline_process(float raw_pm1, float raw_pm25, float raw_pm10,
                                       float raw_co2, float raw_temp, float raw_rh,
                                       processed_metrics_t *output);

#ifdef __cplusplus
}
#endif

#endif /* MEASUREMENT_PIPELINE_H */
