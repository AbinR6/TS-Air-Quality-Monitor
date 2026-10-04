#include "ui_manager.h"
#include "display_driver.h"
#include "esp_log.h"

static const char *TAG = "UI_MGR";

static ui_screen_id_t s_active_screen = SCREEN_DASHBOARD_MAIN;

esp_err_t ui_manager_init(void)
{
    ESP_LOGI(TAG, "Initializing UI manager...");
    display_driver_init();
    ui_render_boot_screen();
    s_active_screen = SCREEN_DASHBOARD_MAIN;
    return ESP_OK;
}

void ui_manager_handle_touch(const touch_event_t *event)
{
    if (event == NULL || !event->touched) return;

    if (event->gesture == TOUCH_GESTURE_TAP || event->gesture == TOUCH_GESTURE_SWIPE_RIGHT) {
        s_active_screen = (ui_screen_id_t)((s_active_screen + 1) % (SCREEN_SYSTEM_INFO + 1));
        ESP_LOGI(TAG, "Screen switched to: %d", s_active_screen);
    } else if (event->gesture == TOUCH_GESTURE_SWIPE_LEFT) {
        if (s_active_screen == SCREEN_BOOT) {
            s_active_screen = SCREEN_SYSTEM_INFO;
        } else {
            s_active_screen = (ui_screen_id_t)(s_active_screen - 1);
        }
        ESP_LOGI(TAG, "Screen switched to: %d", s_active_screen);
    }
}

void ui_manager_update(const processed_metrics_t *metrics, uint8_t battery_pct, bool wifi_connected)
{
    switch (s_active_screen) {
        case SCREEN_BOOT:
            ui_render_boot_screen();
            break;
        case SCREEN_DASHBOARD_MAIN:
            ui_render_main_dashboard(metrics, battery_pct, wifi_connected);
            break;
        case SCREEN_DETAIL_PM:
            ui_render_pm_detail(metrics);
            break;
        case SCREEN_DETAIL_CO2:
            ui_render_co2_detail(metrics);
            break;
        case SCREEN_DETAIL_CLIMATE:
            ui_render_climate_detail(metrics);
            break;
        case SCREEN_SYSTEM_INFO:
            ui_render_system_info("192.168.1.100", "airmonitor-001", battery_pct);
            break;
    }
}
