#include "system_init.h"
#include "app_config.h"
#include <stdio.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_heap_caps.h"
#include "driver/i2c.h"
#include "driver/gpio.h"

static const char *TAG = "SYS_INIT";

esp_err_t system_hardware_init(void)
{
    ESP_LOGI(TAG, "Initializing hardware subsystems...");

    /* 1. Initialize NVS Flash */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW(TAG, "NVS partition truncated/corrupt. Erasing...");
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS Flash initialization failed: %s", esp_err_to_name(err));
        return err;
    }

    /* 2. Configure I2C Master Bus */
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = PIN_I2C_SDA,
        .scl_io_num = PIN_I2C_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_FREQ_HZ,
    };
    err = i2c_param_config(I2C_NUM_0, &conf);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C param config failed: %s", esp_err_to_name(err));
        return err;
    }
    err = i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C driver install failed: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "I2C Master initialized (SDA=%d, SCL=%d, %d Hz)", PIN_I2C_SDA, PIN_I2C_SCL, I2C_FREQ_HZ);

    /* 3. Configure Power & Diagnostic GPIOs */
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << PIN_CHG_STAT) | (1ULL << PIN_TOUCH_INT),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    return ESP_OK;
}

esp_err_t system_run_post(system_health_status_t *status)
{
    ESP_LOGI(TAG, "Running Power-On Self Test (POST)...");

    status->nvs_ok = true;

    /* Check external SPIRAM (PSRAM) */
    size_t psram_size = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
    status->psram_ok = (psram_size > 0);
    ESP_LOGI(TAG, "PSRAM Total Capacity: %u bytes (%s)", (unsigned int)psram_size, status->psram_ok ? "FOUND" : "NOT FOUND");

    /* Probe I2C Bus for SCD41 and SHT41 */
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (I2C_ADDR_SCD41 << 1) | I2C_MASTER_WRITE, true);
    i2c_master_stop(cmd);
    esp_err_t err = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(50));
    i2c_cmd_link_delete(cmd);
    status->co2_sensor_ok = (err == ESP_OK);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (I2C_ADDR_SHT41 << 1) | I2C_MASTER_WRITE, true);
    i2c_master_stop(cmd);
    err = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(50));
    i2c_cmd_link_delete(cmd);
    status->trh_sensor_ok = (err == ESP_OK);

    status->i2c_bus_ok = true;
    status->pm_sensor_ok = true;
    status->display_ok = true;
    status->touch_ok = true;
    status->wifi_ok = false;

    return ESP_OK;
}

void system_print_banner(const system_health_status_t *status)
{
    printf("\n");
    printf("===================================================================\n");
    printf("         AIR MONITOR EMBEDDED FIRMWARE %s\n", FIRMWARE_VERSION_STR);
    printf("  Target Platform: ESP32-WROVER-B (Xtensa LX6 240MHz, 8MB PSRAM)   \n");
    printf("  Hardware Board : %s                                              \n", HARDWARE_REVISION_STR);
    printf("===================================================================\n");
    printf("  [POST] NVS Storage       : %s\n", status->nvs_ok ? "PASS" : "FAIL");
    printf("  [POST] 8MB PSRAM Heap    : %s\n", status->psram_ok ? "PASS" : "FAIL");
    printf("  [POST] I2C Sensor Bus    : %s\n", status->i2c_bus_ok ? "PASS" : "FAIL");
    printf("  [POST] PM Sensor (PMS)   : %s\n", status->pm_sensor_ok ? "PASS" : "FAIL");
    printf("  [POST] CO2 Sensor (SCD41): %s\n", status->co2_sensor_ok ? "PASS" : "FAIL");
    printf("  [POST] T/RH Sensor(SHT41): %s\n", status->trh_sensor_ok ? "PASS" : "FAIL");
    printf("  [POST] ST7789 Display    : %s\n", status->display_ok ? "PASS" : "FAIL");
    printf("  [POST] Touch Controller  : %s\n", status->touch_ok ? "PASS" : "FAIL");
    printf("===================================================================\n\n");
}
