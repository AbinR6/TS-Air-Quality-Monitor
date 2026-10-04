#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "app_config.h"
#include "system_init.h"

static const char *TAG = "MAIN_APP";

/* FreeRTOS Queues */
static QueueHandle_t s_sensor_queue = NULL;
static QueueHandle_t s_telemetry_queue = NULL;

/* Sensor Reading Packet Definition */
typedef struct {
    uint32_t timestamp;
    float pm1_0;
    float pm2_5;
    float pm10;
    float co2_ppm;
    float temperature_c;
    float humidity_rh;
    uint8_t aqi;
    bool valid;
} air_reading_t;

static void sensor_acquisition_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Sensor acquisition task started (Period: %d ms)", SENSOR_POLL_INTERVAL_MS);
    air_reading_t reading;
    uint32_t tick_count = 0;

    while (1) {
        /* Sensor acquisition pipeline routine */
        reading.timestamp = (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS / 1000);
        reading.pm1_0 = 8.5f + (float)(tick_count % 5);
        reading.pm2_5 = 14.2f + (float)(tick_count % 8);
        reading.pm10 = 22.0f + (float)(tick_count % 12);
        reading.co2_ppm = 420.0f + (float)((tick_count * 15) % 350);
        reading.temperature_c = 22.5f + (float)(tick_count % 3) * 0.2f;
        reading.humidity_rh = 48.0f + (float)(tick_count % 5) * 0.5f;
        reading.aqi = (uint8_t)(reading.pm2_5 * 2.1f);
        reading.valid = true;

        if (s_sensor_queue != NULL) {
            xQueueSend(s_sensor_queue, &reading, 0);
        }

        tick_count++;
        vTaskDelay(pdMS_TO_TICKS(SENSOR_POLL_INTERVAL_MS));
    }
}

static void measurement_pipeline_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Measurement pipeline task started");
    air_reading_t rx_reading;

    while (1) {
        if (xQueueReceive(s_sensor_queue, &rx_reading, portMAX_DELAY) == pdTRUE) {
            ESP_LOGD(TAG, "Pipeline received: PM2.5=%.1f, CO2=%.0f ppm, AQI=%u",
                     rx_reading.pm2_5, rx_reading.co2_ppm, rx_reading.aqi);

            if (s_telemetry_queue != NULL) {
                xQueueSend(s_telemetry_queue, &rx_reading, 0);
            }
        }
    }
}

static void ui_render_task(void *pvParameters)
{
    ESP_LOGI(TAG, "UI render task started (~30 FPS)");

    while (1) {
        /* UI State machine update and display dirty rectangle refresh */
        vTaskDelay(pdMS_TO_TICKS(UI_REFRESH_RATE_MS));
    }
}

static void telemetry_network_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Telemetry & network manager task started");
    air_reading_t tx_reading;

    while (1) {
        if (xQueueReceive(s_telemetry_queue, &tx_reading, pdMS_TO_TICKS(TELEMETRY_PUBLISH_INTERVAL_S * 1000)) == pdTRUE) {
            ESP_LOGI(TAG, "[TELEMETRY UPLOAD] PM2.5: %.1f | CO2: %.0f ppm | Temp: %.1f C | RH: %.1f%% | AQI: %u",
                     tx_reading.pm2_5, tx_reading.co2_ppm, tx_reading.temperature_c, tx_reading.humidity_rh, tx_reading.aqi);
        } else {
            ESP_LOGD(TAG, "Telemetry heartbeat tick");
        }
    }
}

static void power_management_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Power management task started");

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(BATTERY_POLL_INTERVAL_MS));
    }
}

void app_main(void)
{
    system_health_status_t health;

    /* 1. Low-level hardware initialization */
    ESP_ERROR_CHECK(system_hardware_init());

    /* 2. Run POST */
    system_run_post(&health);

    /* 3. Output boot banner */
    system_print_banner(&health);

    /* 4. Create Inter-Task Communication Queues */
    s_sensor_queue = xQueueCreate(SENSOR_QUEUE_LEN, sizeof(air_reading_t));
    s_telemetry_queue = xQueueCreate(TELEMETRY_QUEUE_LEN, sizeof(air_reading_t));

    /* 5. Spawn Core FreeRTOS Tasks */
    xTaskCreatePinnedToCore(sensor_acquisition_task, "sensor_task", 4096, NULL, 5, NULL, 1);
    xTaskCreatePinnedToCore(measurement_pipeline_task, "pipe_task", 4096, NULL, 5, NULL, 1);
    xTaskCreatePinnedToCore(ui_render_task, "ui_task", 4096, NULL, 4, NULL, 0);
    xTaskCreatePinnedToCore(telemetry_network_task, "net_task", 6144, NULL, 3, NULL, 0);
    xTaskCreatePinnedToCore(power_management_task, "pwr_task", 2048, NULL, 1, NULL, 0);

    ESP_LOGI(TAG, "All system tasks spawned successfully.");
}
