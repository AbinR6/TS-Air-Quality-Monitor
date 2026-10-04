#ifndef UI_SCREENS_H
#define UI_SCREENS_H

#include "measurement_pipeline.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SCREEN_BOOT = 0,
    SCREEN_DASHBOARD_MAIN,
    SCREEN_DETAIL_PM,
    SCREEN_DETAIL_CO2,
    SCREEN_DETAIL_CLIMATE,
    SCREEN_SYSTEM_INFO
} ui_screen_id_t;

/**
 * @brief Render the boot splash screen.
 */
void ui_render_boot_screen(void);

/**
 * @brief Render the primary overview dashboard.
 */
void ui_render_main_dashboard(const processed_metrics_t *metrics, uint8_t battery_pct, bool wifi_connected);

/**
 * @brief Render particulate matter detail view (PM1.0, PM2.5, PM10).
 */
void ui_render_pm_detail(const processed_metrics_t *metrics);

/**
 * @brief Render CO2 concentration and indoor ventilation warning screen.
 */
void ui_render_co2_detail(const processed_metrics_t *metrics);

/**
 * @brief Render climate conditions (Temperature, Humidity, Comfort Index).
 */
void ui_render_climate_detail(const processed_metrics_t *metrics);

/**
 * @brief Render device diagnostics and network connection status.
 */
void ui_render_system_info(const char *ip_addr, const char *device_id, uint8_t battery_pct);

#ifdef __cplusplus
}
#endif

#endif /* UI_SCREENS_H */
