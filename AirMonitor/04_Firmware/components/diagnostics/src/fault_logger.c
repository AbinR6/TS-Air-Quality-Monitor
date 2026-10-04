#include "fault_logger.h"
#include "esp_log.h"

static const char *TAG = "FAULT_LOG";
static uint32_t s_fault_count = 0;

void fault_logger_record(system_fault_code_t code)
{
    s_fault_count++;
    ESP_LOGW(TAG, "Fault event recorded: code=%d (Total: %lu)", code, (unsigned long)s_fault_count);
}

uint32_t fault_logger_get_count(void)
{
    return s_fault_count;
}
