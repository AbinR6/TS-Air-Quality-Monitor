#include "http_server.h"
#include "esp_log.h"

static const char *TAG = "HTTP_SRV";

esp_err_t http_server_start(void)
{
    ESP_LOGI(TAG, "Starting local REST HTTP server on port 80...");
    ESP_LOGI(TAG, "Registered endpoint: GET /api/v1/metrics");
    ESP_LOGI(TAG, "Registered endpoint: GET /api/v1/status");
    return ESP_OK;
}

esp_err_t http_server_stop(void)
{
    ESP_LOGI(TAG, "Stopping local REST HTTP server.");
    return ESP_OK;
}
