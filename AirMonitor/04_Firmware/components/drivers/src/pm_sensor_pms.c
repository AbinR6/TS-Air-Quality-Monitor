#include "pm_sensor.h"
#include <string.h>
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"

static const char *TAG = "PMS_DRIVER";

#define PMS_UART_PORT   UART_NUM_2
#define PMS_PIN_TX      18
#define PMS_PIN_RX      19
#define PMS_PIN_SET     23
#define PMS_PIN_RESET   5

#define PMS_FRAME_HEADER_1  0x42
#define PMS_FRAME_HEADER_2  0x4D
#define PMS_FRAME_LENGTH    32

esp_err_t pm_sensor_init(void)
{
    ESP_LOGI(TAG, "Initializing PMS5003-compatible UART driver...");

    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    esp_err_t err = uart_param_config(PMS_UART_PORT, &uart_config);
    if (err != ESP_OK) return err;

    err = uart_set_pin(PMS_UART_PORT, PMS_PIN_TX, PMS_PIN_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if (err != ESP_OK) return err;

    err = uart_driver_install(PMS_UART_PORT, 256, 0, 0, NULL, 0);
    if (err != ESP_OK) return err;

    /* Configure SET and RESET pins */
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << PMS_PIN_SET) | (1ULL << PMS_PIN_RESET),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };
    gpio_config(&io_conf);
    gpio_set_level(PMS_PIN_RESET, 1);
    gpio_set_level(PMS_PIN_SET, 1);

    ESP_LOGI(TAG, "PMS UART initialized on GPIO TX=%d, RX=%d", PMS_PIN_TX, PMS_PIN_RX);
    return ESP_OK;
}

esp_err_t pm_sensor_sleep(void)
{
    return gpio_set_level(PMS_PIN_SET, 0);
}

esp_err_t pm_sensor_wake(void)
{
    return gpio_set_level(PMS_PIN_SET, 1);
}

esp_err_t pm_sensor_read(pm_data_t *data)
{
    if (data == NULL) return ESP_ERR_INVALID_ARG;

    uint8_t buffer[PMS_FRAME_LENGTH];
    uint8_t header[2];

    /* Search for frame start 0x42 0x4D */
    while (1) {
        int rx = uart_read_bytes(PMS_UART_PORT, &header[0], 1, pdMS_TO_TICKS(100));
        if (rx <= 0) return ESP_ERR_TIMEOUT;
        if (header[0] == PMS_FRAME_HEADER_1) {
            rx = uart_read_bytes(PMS_UART_PORT, &header[1], 1, pdMS_TO_TICKS(50));
            if (rx > 0 && header[1] == PMS_FRAME_HEADER_2) {
                break; /* Found start of frame */
            }
        }
    }

    buffer[0] = PMS_FRAME_HEADER_1;
    buffer[1] = PMS_FRAME_HEADER_2;
    int remaining = uart_read_bytes(PMS_UART_PORT, &buffer[2], PMS_FRAME_LENGTH - 2, pdMS_TO_TICKS(200));
    if (remaining < (PMS_FRAME_LENGTH - 2)) {
        return ESP_ERR_TIMEOUT;
    }

    /* Verify Checksum (Sum of bytes 0..29 equals 16-bit big-endian word at bytes 30..31) */
    uint16_t checksum = 0;
    for (int i = 0; i < 30; i++) {
        checksum += buffer[i];
    }
    uint16_t expected_checksum = ((uint16_t)buffer[30] << 8) | buffer[31];
    if (checksum != expected_checksum) {
        ESP_LOGW(TAG, "PMS CRC mismatch: computed 0x%04X, expected 0x%04X", checksum, expected_checksum);
        data->valid = false;
        return ESP_ERR_INVALID_CRC;
    }

    /* Unpack registers */
    data->pm1_0_standard    = ((uint16_t)buffer[4] << 8) | buffer[5];
    data->pm2_5_standard    = ((uint16_t)buffer[6] << 8) | buffer[7];
    data->pm10_standard     = ((uint16_t)buffer[8] << 8) | buffer[9];
    data->pm1_0_atmospheric = ((uint16_t)buffer[10] << 8) | buffer[11];
    data->pm2_5_atmospheric = ((uint16_t)buffer[12] << 8) | buffer[13];
    data->pm10_atmospheric  = ((uint16_t)buffer[14] << 8) | buffer[15];
    data->particles_0_3um   = ((uint16_t)buffer[16] << 8) | buffer[17];
    data->particles_0_5um   = ((uint16_t)buffer[18] << 8) | buffer[19];
    data->particles_1_0um   = ((uint16_t)buffer[20] << 8) | buffer[21];
    data->particles_2_5um   = ((uint16_t)buffer[22] << 8) | buffer[23];
    data->particles_5_0um   = ((uint16_t)buffer[24] << 8) | buffer[25];
    data->particles_10um    = ((uint16_t)buffer[26] << 8) | buffer[27];
    data->valid = true;

    return ESP_OK;
}
