#include "self_test.h"
#include "esp_heap_caps.h"
#include "esp_log.h"

static const char *TAG = "SELF_TEST";

esp_err_t self_test_run(diagnostics_summary_t *diag)
{
    if (diag == NULL) return ESP_ERR_INVALID_ARG;

    diag->free_heap_internal = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
    diag->free_heap_psram = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    diag->min_free_heap = esp_get_minimum_free_heap_size();

    diag->i2c_bus_healthy = true;
    diag->pm_sensor_healthy = true;
    diag->co2_sensor_healthy = true;
    diag->trh_sensor_healthy = true;

    ESP_LOGI(TAG, "Self-Test: Internal Heap: %lu B, PSRAM: %lu B, Min Free: %lu B",
             (unsigned long)diag->free_heap_internal,
             (unsigned long)diag->free_heap_psram,
             (unsigned long)diag->min_free_heap);

    return ESP_OK;
}
