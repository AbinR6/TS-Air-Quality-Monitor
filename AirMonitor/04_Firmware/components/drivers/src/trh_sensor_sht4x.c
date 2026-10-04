#include "trh_sensor.h"
#include <string.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "SHT4X_DRIVER";

#define SHT4X_I2C_PORT      I2C_NUM_0
#define SHT4X_I2C_ADDR      0x44
#define SHT4X_CMD_MEAS_HIGH 0xFD

static uint8_t sht4x_crc8(const uint8_t *data, uint16_t count)
{
    uint8_t crc = 0xFF;
    for (uint16_t current_byte = 0; current_byte < count; ++current_byte) {
        crc ^= data[current_byte];
        for (uint8_t crc_bit = 8; crc_bit > 0; --crc_bit) {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc = (crc << 1);
        }
    }
    return crc;
}

esp_err_t trh_sensor_init(void)
{
    ESP_LOGI(TAG, "Initializing Sensirion SHT4x candidate T/RH driver...");
    return ESP_OK;
}

esp_err_t trh_sensor_read(trh_data_t *data)
{
    if (data == NULL) return ESP_ERR_INVALID_ARG;

    /* Send measure command */
    uint8_t cmd_byte = SHT4X_CMD_MEAS_HIGH;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SHT4X_I2C_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, cmd_byte, true);
    i2c_master_stop(cmd);
    esp_err_t err = i2c_master_cmd_begin(SHT4X_I2C_PORT, cmd, pdMS_TO_TICKS(50));
    i2c_cmd_link_delete(cmd);
    if (err != ESP_OK) return err;

    /* Measurement takes ~8.2ms for high precision */
    vTaskDelay(pdMS_TO_TICKS(10));

    uint8_t rx_buf[6];
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SHT4X_I2C_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, rx_buf, 6, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    err = i2c_master_cmd_begin(SHT4X_I2C_PORT, cmd, pdMS_TO_TICKS(50));
    i2c_cmd_link_delete(cmd);
    if (err != ESP_OK) return err;

    if (sht4x_crc8(&rx_buf[0], 2) != rx_buf[2] ||
        sht4x_crc8(&rx_buf[3], 2) != rx_buf[5]) {
        ESP_LOGW(TAG, "SHT4x CRC8 mismatch.");
        data->valid = false;
        return ESP_ERR_INVALID_CRC;
    }

    uint16_t t_ticks = ((uint16_t)rx_buf[0] << 8) | rx_buf[1];
    uint16_t rh_ticks = ((uint16_t)rx_buf[3] << 8) | rx_buf[4];

    data->temperature_c = -45.0f + 175.0f * (float)t_ticks / 65535.0f;
    float rh = -6.0f + 125.0f * (float)rh_ticks / 65535.0f;
    if (rh < 0.0f) rh = 0.0f;
    if (rh > 100.0f) rh = 100.0f;
    data->humidity_rh = rh;
    data->valid = true;

    return ESP_OK;
}
