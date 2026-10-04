#include "display_driver.h"
#include <string.h>
#include "esp_log.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "ST7789_DRIVER";

#define LCD_HOST        SPI2_HOST
#define PIN_MOSI        27
#define PIN_SCLK        26
#define PIN_CS          25
#define PIN_DC          33
#define PIN_RST         32
#define PIN_BACKLIGHT   14

static spi_device_handle_t s_spi = NULL;

static void lcd_send_cmd(uint8_t cmd)
{
    gpio_set_level(PIN_DC, 0);
    spi_transaction_t t = {
        .length = 8,
        .tx_buffer = &cmd,
    };
    spi_device_polling_transmit(s_spi, &t);
}

static void lcd_send_data(const uint8_t *data, int len)
{
    if (len == 0) return;
    gpio_set_level(PIN_DC, 1);
    spi_transaction_t t = {
        .length = (size_t)len * 8,
        .tx_buffer = data,
    };
    spi_device_polling_transmit(s_spi, &t);
}

static void lcd_send_byte(uint8_t val)
{
    lcd_send_data(&val, 1);
}

esp_err_t display_driver_init(void)
{
    ESP_LOGI(TAG, "Initializing ST7789 SPI LCD driver...");

    /* 1. Configure Control GPIOs */
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << PIN_DC) | (1ULL << PIN_RST),
        .mode = GPIO_MODE_OUTPUT,
    };
    gpio_config(&io_conf);

    /* 2. Configure Backlight PWM via LEDC */
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_8_BIT,
        .freq_hz          = 5000,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .timer_sel      = LEDC_TIMER_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = PIN_BACKLIGHT,
        .duty           = 200, /* ~80% brightness default */
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);

    /* 3. Initialize SPI Bus */
    spi_bus_config_t buscfg = {
        .mosi_io_num = PIN_MOSI,
        .miso_io_num = -1,
        .sclk_io_num = PIN_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = LCD_WIDTH * 40 * sizeof(uint16_t),
    };
    esp_err_t ret = spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) return ret;

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 40000000, /* 40 MHz */
        .mode = 0,
        .spics_io_num = PIN_CS,
        .queue_size = 7,
    };
    ret = spi_bus_add_device(LCD_HOST, &devcfg, &s_spi);
    if (ret != ESP_OK) return ret;

    /* 4. Hardware Reset */
    gpio_set_level(PIN_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(20));
    gpio_set_level(PIN_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(120));

    /* 5. ST7789 Initialization Sequence */
    lcd_send_cmd(0x01); /* Software Reset */
    vTaskDelay(pdMS_TO_TICKS(150));

    lcd_send_cmd(0x11); /* Sleep Out */
    vTaskDelay(pdMS_TO_TICKS(120));

    lcd_send_cmd(0x3A); /* Pixel Format */
    lcd_send_byte(0x55); /* 16-bit / pixel RGB565 */

    lcd_send_cmd(0x36); /* Memory Access Control */
    lcd_send_byte(0x00); /* Normal orientation */

    lcd_send_cmd(0x21); /* Display Inversion On */
    lcd_send_cmd(0x13); /* Normal Display Mode On */
    lcd_send_cmd(0x29); /* Display On */
    vTaskDelay(pdMS_TO_TICKS(50));

    display_clear(0x0000); /* Clear screen to Black */
    ESP_LOGI(TAG, "ST7789 display initialized successfully.");
    return ESP_OK;
}

esp_err_t display_set_brightness(uint8_t percent)
{
    if (percent > 100) percent = 100;
    uint32_t duty = (percent * 255) / 100;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
    return ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

esp_err_t display_flush_rect(int x0, int y0, int x1, int y1, const uint16_t *color_data)
{
    if (x0 < 0 || y0 < 0 || x1 >= LCD_WIDTH || y1 >= LCD_HEIGHT) return ESP_ERR_INVALID_ARG;

    /* Set Column Address */
    lcd_send_cmd(0x2A);
    uint8_t col_data[4] = { (uint8_t)(x0 >> 8), (uint8_t)(x0 & 0xFF), (uint8_t)(x1 >> 8), (uint8_t)(x1 & 0xFF) };
    lcd_send_data(col_data, 4);

    /* Set Row Address */
    lcd_send_cmd(0x2B);
    uint8_t row_data[4] = { (uint8_t)(y0 >> 8), (uint8_t)(y0 & 0xFF), (uint8_t)(y1 >> 8), (uint8_t)(y1 & 0xFF) };
    lcd_send_data(row_data, 4);

    /* Memory Write */
    lcd_send_cmd(0x2C);
    int len = (x1 - x0 + 1) * (y1 - y0 + 1) * 2;
    lcd_send_data((const uint8_t *)color_data, len);

    return ESP_OK;
}

esp_err_t display_clear(uint16_t color)
{
    uint16_t line_buf[LCD_WIDTH];
    for (int i = 0; i < LCD_WIDTH; i++) {
        line_buf[i] = (color >> 8) | (color << 8); /* Endian swap for SPI */
    }
    for (int y = 0; y < LCD_HEIGHT; y++) {
        display_flush_rect(0, y, LCD_WIDTH - 1, y, line_buf);
    }
    return ESP_OK;
}
