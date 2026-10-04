#ifndef TELEMETRY_ENCODER_H
#define TELEMETRY_ENCODER_H

#include "measurement_pipeline.h"
#include <stddef.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Encode processed metrics into compact JSON telemetry payload.
 * @param[in] metrics Pointer to processed metrics.
 * @param[in] device_id Device serial/identifier string.
 * @param[in] battery_pct Current battery state of charge (0-100%).
 * @param[in] seq_num Incremental sequence counter.
 * @param[out] buffer Output char buffer for JSON string.
 * @param[in] buffer_size Maximum capacity of output buffer.
 * @return ESP_OK on success, or ESP_ERR_NO_MEM if truncated.
 */
esp_err_t telemetry_encode_json(const processed_metrics_t *metrics,
                                const char *device_id,
                                uint8_t battery_pct,
                                uint32_t seq_num,
                                char *buffer,
                                size_t buffer_size);

#ifdef __cplusplus
}
#endif

#endif /* TELEMETRY_ENCODER_H */
