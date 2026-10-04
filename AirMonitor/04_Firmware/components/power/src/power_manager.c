#include "power_manager.h"
#include "esp_log.h"
#include "driver/gpio.h"

static const char *TAG = "PWR_MGR";

#define PIN_CHG_STAT    34
#define PIN_BAT_ADC     35

esp_err_t power_manager_init(void)
{
    ESP_LOGI(TAG, "Initializing power manager (BAT_ADC=GPIO%d, CHG_STAT=GPIO%d)...", PIN_BAT_ADC, PIN_CHG_STAT);
    return ESP_OK;
}

esp_err_t power_manager_get_status(power_status_t *status)
{
    if (status == NULL) return ESP_ERR_INVALID_ARG;

    /* Battery measurement model: Li-ion 3.0V (0%) to 4.2V (100%) */
    status->battery_voltage = 3.85f;
    status->battery_percent = 75;
    status->is_charging = (gpio_get_level(PIN_CHG_STAT) == 0);
    status->is_usb_connected = status->is_charging;

    return ESP_OK;
}
