#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LCD_WIDTH   240
#define LCD_HEIGHT  320

/**
 * @brief Initialize the SPI bus, GPIO controls, PWM backlight, and ST7789 controller.
 */
esp_err_t display_driver_init(void);

/**
 * @brief Set display backlight brightness level.
 * @param[in] percent Brightness percentage (0 to 100).
 */
esp_err_t display_set_brightness(uint8_t percent);

/**
 * @brief Send a rectangular pixel buffer to the display over high-speed SPI.
 * @param[in] x0 Starting X coordinate.
 * @param[in] y0 Starting Y coordinate.
 * @param[in] x1 Ending X coordinate.
 * @param[in] y1 Ending Y coordinate.
 * @param[in] color_data Array of RGB565 16-bit color values.
 */
esp_err_t display_flush_rect(int x0, int y0, int x1, int y1, const uint16_t *color_data);

/**
 * @brief Clear entire display with specified background color (RGB565).
 */
esp_err_t display_clear(uint16_t color);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_DRIVER_H */
