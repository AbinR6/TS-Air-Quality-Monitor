#include "ui_screens.h"
#include "ui_theme.h"
#include "display_driver.h"
#include "air_quality_index.h"
#include "esp_log.h"
#include <stdio.h>

static const char *TAG = "UI_SCREENS";

void ui_render_boot_screen(void)
{
    ESP_LOGD(TAG, "Rendering BOOT splash...");
    display_clear(COLOR_BG_DARK);
}

void ui_render_main_dashboard(const processed_metrics_t *metrics, uint8_t battery_pct, bool wifi_connected)
{
    if (metrics == NULL) return;

    /* High-contrast engineering dashboard */
    ESP_LOGD(TAG, "Render Main Dashboard: AQI=%u, PM2.5=%.1f, CO2=%.0f, Bat=%u%%",
             metrics->aqi, metrics->pm2_5, metrics->co2_ppm, battery_pct);
}

void ui_render_pm_detail(const processed_metrics_t *metrics)
{
    if (metrics == NULL) return;
    ESP_LOGD(TAG, "Render PM Detail: PM1.0=%.1f, PM2.5=%.1f, PM10=%.1f",
             metrics->pm1_0, metrics->pm2_5, metrics->pm10);
}

void ui_render_co2_detail(const processed_metrics_t *metrics)
{
    if (metrics == NULL) return;
    ESP_LOGD(TAG, "Render CO2 Detail: CO2=%.0f ppm", metrics->co2_ppm);
}

void ui_render_climate_detail(const processed_metrics_t *metrics)
{
    if (metrics == NULL) return;
    ESP_LOGD(TAG, "Render Climate Detail: Temp=%.1f C, RH=%.1f%%",
             metrics->temperature_c, metrics->humidity_rh);
}

void ui_render_system_info(const char *ip_addr, const char *device_id, uint8_t battery_pct)
{
    ESP_LOGD(TAG, "Render System Info: IP=%s, ID=%s, Bat=%u%%",
             ip_addr ? ip_addr : "0.0.0.0", device_id ? device_id : "UNKNOWN", battery_pct);
}
