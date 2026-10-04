#include "telemetry_buffer.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

static buffered_record_t s_buffer[TELEMETRY_BUFFER_CAPACITY];
static size_t s_head = 0;
static size_t s_tail = 0;
static size_t s_count = 0;
static SemaphoreHandle_t s_lock = NULL;

esp_err_t telemetry_buffer_init(void)
{
    s_head = 0;
    s_tail = 0;
    s_count = 0;
    if (s_lock == NULL) {
        s_lock = xSemaphoreCreateMutex();
    }
    return ESP_OK;
}

esp_err_t telemetry_buffer_push(const buffered_record_t *record)
{
    if (record == NULL || s_lock == NULL) return ESP_ERR_INVALID_ARG;

    xSemaphoreTake(s_lock, portMAX_DELAY);
    s_buffer[s_head] = *record;
    s_head = (s_head + 1) % TELEMETRY_BUFFER_CAPACITY;

    if (s_count < TELEMETRY_BUFFER_CAPACITY) {
        s_count++;
    } else {
        /* Buffer is full: advance tail to discard oldest */
        s_tail = (s_tail + 1) % TELEMETRY_BUFFER_CAPACITY;
    }
    xSemaphoreGive(s_lock);
    return ESP_OK;
}

esp_err_t telemetry_buffer_pop(buffered_record_t *record)
{
    if (record == NULL || s_lock == NULL) return ESP_ERR_INVALID_ARG;

    xSemaphoreTake(s_lock, portMAX_DELAY);
    if (s_count == 0) {
        xSemaphoreGive(s_lock);
        return ESP_ERR_NOT_FOUND;
    }

    *record = s_buffer[s_tail];
    s_tail = (s_tail + 1) % TELEMETRY_BUFFER_CAPACITY;
    s_count--;
    xSemaphoreGive(s_lock);
    return ESP_OK;
}

size_t telemetry_buffer_count(void)
{
    size_t count = 0;
    if (s_lock != NULL) {
        xSemaphoreTake(s_lock, portMAX_DELAY);
        count = s_count;
        xSemaphoreGive(s_lock);
    }
    return count;
}
