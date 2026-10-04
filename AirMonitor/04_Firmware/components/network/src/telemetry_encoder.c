#include "telemetry_encoder.h"
#include <stdio.h>

esp_err_t telemetry_encode_json(const processed_metrics_t *metrics,
                                const char *device_id,
                                uint8_t battery_pct,
                                uint32_t seq_num,
                                char *buffer,
                                size_t buffer_size)
{
    if (metrics == NULL || buffer == NULL || buffer_size == 0) {
        return ESP_ERR_INVALID_ARG;
    }

    int written = snprintf(buffer, buffer_size,
        "{"
            "\"device_id\":\"%s\","
            "\"seq\":%lu,"
            "\"timestamp\":%lu,"
            "\"sensors\":{"
                "\"pm1_0\":%.1f,"
                "\"pm2_5\":%.1f,"
                "\"pm10\":%.1f,"
                "\"co2_ppm\":%.0f,"
                "\"temperature_c\":%.2f,"
                "\"humidity_rh\":%.2f,"
                "\"aqi\":%u"
            "},"
            "\"validity\":{"
                "\"pm\":%s,"
                "\"co2\":%s,"
                "\"trh\":%s"
            "},"
            "\"diagnostics\":{"
                "\"battery_pct\":%u"
            "}"
        "}",
        device_id ? device_id : "airmonitor-001",
        (unsigned long)seq_num,
        (unsigned long)metrics->timestamp,
        metrics->pm1_0,
        metrics->pm2_5,
        metrics->pm10,
        metrics->co2_ppm,
        metrics->temperature_c,
        metrics->humidity_rh,
        metrics->aqi,
        metrics->pm_valid ? "true" : "false",
        metrics->co2_valid ? "true" : "false",
        metrics->trh_valid ? "true" : "false",
        battery_pct
    );

    if (written < 0 || (size_t)written >= buffer_size) {
        return ESP_ERR_NO_MEM;
    }

    return ESP_OK;
}
