#ifndef TELEMETRY_BUFFER_H
#define TELEMETRY_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TELEMETRY_BUFFER_CAPACITY 256

typedef struct {
    uint32_t timestamp;
    float pm2_5;
    float co2_ppm;
    float temperature_c;
    float humidity_rh;
    uint16_t aqi;
} buffered_record_t;

/**
 * @brief Initialize in-memory / flash ring buffer.
 */
esp_err_t telemetry_buffer_init(void);

/**
 * @brief Push a record to the buffer (overwrites oldest if full).
 */
esp_err_t telemetry_buffer_push(const buffered_record_t *record);

/**
 * @brief Pop the oldest record from the buffer.
 */
esp_err_t telemetry_buffer_pop(buffered_record_t *record);

/**
 * @brief Return current number of buffered records.
 */
size_t telemetry_buffer_count(void);

#ifdef __cplusplus
}
#endif

#endif /* TELEMETRY_BUFFER_H */
