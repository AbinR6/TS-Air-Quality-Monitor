#include "touch_driver.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "TOUCH_DRIVER";

#define TOUCH_I2C_PORT      I2C_NUM_0
#define TOUCH_I2C_ADDR      0x15
#define PIN_TOUCH_INT       13
#define PIN_TOUCH_RST       4

esp_err_t touch_driver_init(void)
{
    ESP_LOGI(TAG, "Initializing DANY_TOUCH capacitive slider driver...");

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << PIN_TOUCH_RST),
        .mode = GPIO_MODE_OUTPUT,
    };
    gpio_config(&io_conf);

    /* Hardware reset touch controller */
    gpio_set_level(PIN_TOUCH_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(PIN_TOUCH_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(50));

    ESP_LOGI(TAG, "Touch controller reset sequence complete.");
    return ESP_OK;
}

esp_err_t touch_driver_read(touch_event_t *event)
{
    if (event == NULL) return ESP_ERR_INVALID_ARG;

    event->gesture = TOUCH_GESTURE_NONE;
    event->touched = false;

    /* Check if INT line is asserted low */
    if (gpio_get_level(PIN_TOUCH_INT) == 1) {
        return ESP_OK; /* No active touch */
    }

    uint8_t data[6];
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (TOUCH_I2C_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x01, true); /* Gesture register */
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (TOUCH_I2C_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, data, 6, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    esp_err_t err = i2c_master_cmd_begin(TOUCH_I2C_PORT, cmd, pdMS_TO_TICKS(50));
    i2c_cmd_link_delete(cmd);

    if (err == ESP_OK) {
        uint8_t gesture_id = data[0];
        event->touched = (data[1] > 0); /* Touch points */
        event->x = ((data[2] & 0x0F) << 8) | data[3];
        event->y = ((data[4] & 0x0F) << 8) | data[5];

        switch (gesture_id) {
            case 0x01: event->gesture = TOUCH_GESTURE_SWIPE_LEFT; break;
            case 0x02: event->gesture = TOUCH_GESTURE_SWIPE_RIGHT; break;
            case 0x05: event->gesture = TOUCH_GESTURE_TAP; break;
            case 0x0C: event->gesture = TOUCH_GESTURE_LONG_PRESS; break;
            default:   event->gesture = TOUCH_GESTURE_NONE; break;
        }
    }
    return err;
}
