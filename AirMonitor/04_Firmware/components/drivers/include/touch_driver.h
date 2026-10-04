#ifndef TOUCH_DRIVER_H
#define TOUCH_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TOUCH_GESTURE_NONE = 0,
    TOUCH_GESTURE_TAP,
    TOUCH_GESTURE_SWIPE_LEFT,
    TOUCH_GESTURE_SWIPE_RIGHT,
    TOUCH_GESTURE_LONG_PRESS,
} touch_gesture_t;

typedef struct {
    touch_gesture_t gesture;
    uint16_t x;
    uint16_t y;
    bool touched;
} touch_event_t;

/**
 * @brief Initialize capacitive touch controller on J2.
 */
esp_err_t touch_driver_init(void);

/**
 * @brief Poll current touch status and gesture.
 * @param[out] event Output touch event structure.
 */
esp_err_t touch_driver_read(touch_event_t *event);

#ifdef __cplusplus
}
#endif

#endif /* TOUCH_DRIVER_H */
