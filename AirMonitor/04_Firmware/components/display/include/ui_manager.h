#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "ui_screens.h"
#include "touch_driver.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize UI manager, screen controller, and display drivers.
 */
esp_err_t ui_manager_init(void);

/**
 * @brief Handle touch gestures from DANY_TOUCH top bar to cycle or select screens.
 */
void ui_manager_handle_touch(const touch_event_t *event);

/**
 * @brief Step UI refresh cycle.
 */
void ui_manager_update(const processed_metrics_t *metrics, uint8_t battery_pct, bool wifi_connected);

#ifdef __cplusplus
}
#endif

#endif /* UI_MANAGER_H */
