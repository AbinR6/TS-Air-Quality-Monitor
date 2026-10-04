#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================= */
/* System Identification & Versioning                                        */
/* ========================================================================= */
#define FIRMWARE_VERSION_MAJOR      1
#define FIRMWARE_VERSION_MINOR      0
#define FIRMWARE_VERSION_PATCH      0
#define FIRMWARE_VERSION_STR        "v1.0.0-recon"
#define HARDWARE_REVISION_STR       "DANY_JPU_MB_P1_ESP32"

/* ========================================================================= */
/* Hardware GPIO Pin Assignments (ESP32-WROVER-B)                            */
/* ========================================================================= */

/* I2C Master Bus (Sensors & Touch) */
#define PIN_I2C_SDA                 21
#define PIN_I2C_SCL                 22
#define I2C_FREQ_HZ                 400000

/* I2C Sensor Addresses */
#define I2C_ADDR_SCD41              0x62
#define I2C_ADDR_SHT41              0x44
#define I2C_ADDR_CST816S            0x15

/* Particulate Matter Sensor (PMS5003 Candidate) - UART2 */
#define PIN_PM_UART_TX              18
#define PIN_PM_UART_RX              19
#define PIN_PM_SET                  23
#define PIN_PM_RESET                5
#define PM_UART_BAUD_RATE           9600

/* SPI Display Interface (ST7789 IPS LCD) - SPI2 (HSPI) */
#define PIN_DISP_MOSI               27
#define PIN_DISP_SCLK               26
#define PIN_DISP_CS                 25
#define PIN_DISP_DC                 33
#define PIN_DISP_RST                32
#define PIN_DISP_BACKLIGHT          14
#define DISP_SPI_CLOCK_SPEED_HZ     40000000

/* Capacitive Touch Interface (DANY_TOUCH) */
#define PIN_TOUCH_INT               13
#define PIN_TOUCH_RST               4

/* Power Management & Battery Monitoring */
#define PIN_BAT_ADC                 35  /* ADC1 Channel 7 */
#define PIN_CHG_STAT                34  /* Digital Input: Active Low Charging */
#define PIN_PWR_KEY                 0   /* Boot strapping / User button */

/* ========================================================================= */
/* Operational Timing & Queue Depths                                         */
/* ========================================================================= */
#define SENSOR_POLL_INTERVAL_MS     1000
#define TELEMETRY_PUBLISH_INTERVAL_S 60
#define UI_REFRESH_RATE_MS          33   /* ~30 FPS */
#define BATTERY_POLL_INTERVAL_MS    5000

#define SENSOR_QUEUE_LEN            10
#define UI_EVENT_QUEUE_LEN          16
#define TELEMETRY_QUEUE_LEN         32

#ifdef __cplusplus
}
#endif

#endif /* APP_CONFIG_H */
