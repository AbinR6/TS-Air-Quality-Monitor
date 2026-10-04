#include "co2_sensor.h"
#include <string.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "SCD4X_DRIVER";

#define SCD4X_I2C_PORT      I2C_NUM_0
#define SCD4X_I2C_ADDR      0x62

#define CMD_START_PERIODIC  0x21B1
#define CMD_READ_MEASURE    0xEC05
#define CMD_STOP_PERIODIC   0x3F86

static uint8_t sensirion_crc8(const uint8_t *data, uint16_t count)
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

static esp_err_t scd4x_send_command(uint16_t command)
{
    uint8_t buf[2] = { (uint8_t)(command >> 8), (uint8_t)(command & 0xFF) };
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SCD4X_I2C_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(cmd, buf, 2, true);
    i2c_master_stop(cmd);
    esp_err_t err = i2c_master_cmd_begin(SCD4X_I2C_PORT, cmd, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(cmd);
    return err;
}

esp_err_t co2_sensor_init(void)
{
    ESP_LOGI(TAG, "Initializing Sensirion SCD4x candidate CO2 driver...");
    esp_err_t err = scd4x_send_command(CMD_START_PERIODIC);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "SCD4x started continuous periodic sampling.");
    } else {
        ESP_LOGW(TAG, "SCD4x not detected on I2C address 0x%02X (%s)", SCD4X_I2C_ADDR, esp_err_to_name(err));
    }
    return err;
}

esp_err_t co2_sensor_stop(void)
{
    return scd4x_send_command(CMD_STOP_PERIODIC);
}

esp_err_t co2_sensor_read(co2_data_t *data)
{
    if (data == NULL) return ESP_ERR_INVALID_ARG;

    esp_err_t err = scd4x_send_command(CMD_READ_MEASURE);
    if (err != ESP_OK) return err;

    vTaskDelay(pdMS_TO_TICKS(5));

    uint8_t rx_buf[9];
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SCD4X_I2C_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, rx_buf, 9, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    err = i2c_master_cmd_begin(SCD4X_I2C_PORT, cmd, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(cmd);
    if (err != ESP_OK) return err;

    /* Verify CRC for CO2, Temp, and RH words */
    if (sensirion_crc8(&rx_buf[0], 2) != rx_buf[2] ||
        sensirion_crc8(&rx_buf[3], 2) != rx_buf[5] ||
        sensirion_crc8(&rx_buf[6], 2) != rx_buf[8]) {
        ESP_LOGW(TAG, "SCD4x CRC8 validation failed.");
        data->valid = false;
        return ESP_ERR_INVALID_CRC;
    }

    uint16_t raw_co2 = ((uint16_t)rx_buf[0] << 8) | rx_buf[1];
    uint16_t raw_temp = ((uint16_t)rx_buf[3] << 8) | rx_buf[4];
    uint16_t raw_rh   = ((uint16_t)rx_buf[6] << 8) | rx_buf[7];

    data->co2_ppm = raw_co2;
    data->temperature_c = -45.0f + 175.0f * (float)raw_temp / 65536.0f;
    data->humidity_rh = 100.0f * (float)raw_rh / 65536.0f;
    data->valid = true;

    return ESP_OK;
}
