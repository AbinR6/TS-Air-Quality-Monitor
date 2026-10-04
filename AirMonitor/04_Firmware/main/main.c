#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "app_config.h"
#include "system_init.h"

/* FreeRTOS Kernel Includes (when using FreeRTOS with STM32 HAL) */
#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

static QueueHandle_t s_sensor_queue = NULL;
static QueueHandle_t s_telemetry_queue = NULL;
#endif

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

#ifdef USE_FREERTOS
static void sensor_acquisition_task(void *pvParameters)
{
    printf("[TASK] Sensor acquisition task started (Period: %d ms)\n", SENSOR_POLL_INTERVAL_MS);
    air_reading_t reading;
    uint32_t tick_count = 0;

    while (1) {
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
    printf("[TASK] Measurement processing task started\n");
    air_reading_t rx_reading;

    while (1) {
        if (xQueueReceive(s_sensor_queue, &rx_reading, portMAX_DELAY) == pdTRUE) {
            if (s_telemetry_queue != NULL) {
                xQueueSend(s_telemetry_queue, &rx_reading, 0);
            }
        }
    }
}

static void ui_render_task(void *pvParameters)
{
    printf("[TASK] UI render task started (~30 FPS)\n");
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(UI_REFRESH_RATE_MS));
    }
}

static void telemetry_comm_task(void *pvParameters)
{
    printf("[TASK] Telemetry communication task started\n");
    air_reading_t tx_reading;

    while (1) {
        if (xQueueReceive(s_telemetry_queue, &tx_reading, pdMS_TO_TICKS(TELEMETRY_PUBLISH_INTERVAL_S * 1000)) == pdTRUE) {
            printf("[TELEMETRY] PM2.5: %.1f | CO2: %.0f ppm | Temp: %.1f C | RH: %.1f%% | AQI: %u\n",
                   tx_reading.pm2_5, tx_reading.co2_ppm, tx_reading.temperature_c, 
                   tx_reading.humidity_rh, tx_reading.aqi);
        }
    }
}

static void power_management_task(void *pvParameters)
{
    printf("[TASK] Power management task started\n");
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(BATTERY_POLL_INTERVAL_MS));
    }
}
#endif /* USE_FREERTOS */

/**
 * @brief Main firmware entry point for STM32F407ZGT6.
 */
int main(void)
{
    system_health_status_t health;

    /* 1. Low-level hardware initialization (Clocks, GPIOs, Buses) */
    system_hardware_init();

    /* 2. Execute Power-On Self Test (POST) */
    system_run_post(&health);

    /* 3. Output boot diagnostic banner to serial console */
    system_print_banner(&health);

#ifdef USE_FREERTOS
    /* 4. Create Inter-Task Communication Queues */
    s_sensor_queue = xQueueCreate(SENSOR_QUEUE_LEN, sizeof(air_reading_t));
    s_telemetry_queue = xQueueCreate(TELEMETRY_QUEUE_LEN, sizeof(air_reading_t));

    /* 5. Spawn Core Tasks (Single-core ARM Cortex-M4 scheduler) */
    xTaskCreate(sensor_acquisition_task, "sensor_task", 512, NULL, 3, NULL);
    xTaskCreate(measurement_pipeline_task, "pipe_task", 512, NULL, 3, NULL);
    xTaskCreate(ui_render_task, "ui_task", 512, NULL, 2, NULL);
    xTaskCreate(telemetry_comm_task, "comm_task", 512, NULL, 2, NULL);
    xTaskCreate(power_management_task, "pwr_task", 256, NULL, 1, NULL);

    printf("[MAIN] Starting FreeRTOS task scheduler...\n");
    vTaskStartScheduler();
#else
    printf("[MAIN] Superloop execution started (Bare-Metal HAL mode)\n");
    while (1) {
        /* Bare-metal polling loop fallback */
    }
#endif

    /* Should never reach here */
    while (1) {
    }
    return 0;
}
