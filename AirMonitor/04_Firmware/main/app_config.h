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
#define FIRMWARE_VERSION_STR        "v1.0.0"
#define HARDWARE_REVISION_STR       "DANY_JPU_MB_P1_STM32"

/* ========================================================================= */
/* Microcontroller Hardware Specifications (Authoritative)                   */
/* ========================================================================= */
#define MCU_DEVICE_NAME             "STM32F407ZGT6"
#define MCU_CORE_NAME               "ARM Cortex-M4 with FPU"
#define MCU_MAX_CLOCK_HZ            168000000UL  /* 168 MHz Core Clock */
#define MCU_FLASH_SIZE_BYTES        (1024UL * 1024UL)  /* 1 MB Flash */
#define MCU_SRAM_SIZE_BYTES         (192UL * 1024UL)   /* 192 KB SRAM */
#define MCU_PACKAGE                 "LQFP-144"
#define MCU_MANUFACTURER            "STMicroelectronics"

/* ========================================================================= */
/* Peripheral & Pin Mapping (Candidate / Pending Physical Trace Verification)*/
/* ========================================================================= */

/* I2C Master Bus (Sensors & Touch) - Candidate Peripheral: I2C1 */
#define AIR_I2C_PERIPH              I2C1
#define AIR_I2C_FREQ_HZ             400000  /* Fast-Mode 400 kHz */
#define AIR_I2C_SCL_PORT            GPIOB
#define AIR_I2C_SCL_PIN             GPIO_PIN_6   /* Candidate (alt: PB8) */
#define AIR_I2C_SDA_PORT            GPIOB
#define AIR_I2C_SDA_PIN             GPIO_PIN_7   /* Candidate (alt: PB9) */

/* I2C Sensor 7-bit Device Addresses */
#define I2C_ADDR_SCD41              0x62
#define I2C_ADDR_SHT41              0x44
#define I2C_ADDR_CST816S            0x15

/* Particulate Matter Sensor (PMS5003 Candidate) - Candidate Peripheral: USART2 */
#define AIR_PM_UART                 USART2
#define AIR_PM_UART_BAUD            9600
#define AIR_PM_TX_PORT              GPIOA
#define AIR_PM_TX_PIN               GPIO_PIN_2   /* Candidate (alt: PD5) */
#define AIR_PM_RX_PORT              GPIOA
#define AIR_PM_RX_PIN               GPIO_PIN_3   /* Candidate (alt: PD6) */
#define AIR_PM_SET_PORT             GPIOC
#define AIR_PM_SET_PIN              GPIO_PIN_4   /* Candidate (alt: PD3) */
#define AIR_PM_RESET_PORT           GPIOC
#define AIR_PM_RESET_PIN            GPIO_PIN_5   /* Candidate (alt: PD4) */

/* SPI Display Interface (ST7789 IPS LCD) - Candidate Peripheral: SPI1 */
#define AIR_DISP_SPI                SPI1
#define AIR_DISP_SPI_BAUD_DIV       SPI_BAUDRATEPRESCALER_4  /* ~21 MHz */
#define AIR_DISP_SCK_PORT           GPIOA
#define AIR_DISP_SCK_PIN            GPIO_PIN_5   /* Candidate (alt: PB13) */
#define AIR_DISP_MOSI_PORT          GPIOA
#define AIR_DISP_MOSI_PIN           GPIO_PIN_7   /* Candidate (alt: PB15) */
#define AIR_DISP_CS_PORT            GPIOA
#define AIR_DISP_CS_PIN             GPIO_PIN_4   /* Candidate (alt: PB12) */
#define AIR_DISP_DC_PORT            GPIOC
#define AIR_DISP_DC_PIN             GPIO_PIN_1   /* Candidate (alt: PE2) */
#define AIR_DISP_RST_PORT           GPIOC
#define AIR_DISP_RST_PIN            GPIO_PIN_2   /* Candidate (alt: PE3) */
#define AIR_DISP_BL_PORT            GPIOB
#define AIR_DISP_BL_PIN             GPIO_PIN_0   /* Candidate: TIM3_CH3 PWM */

/* Capacitive Touch Interface (DANY_TOUCH) */
#define AIR_TOUCH_INT_PORT          GPIOC
#define AIR_TOUCH_INT_PIN           GPIO_PIN_0   /* Candidate: EXTI0 */
#define AIR_TOUCH_RST_PORT          GPIOC
#define AIR_TOUCH_RST_PIN           GPIO_PIN_3   /* Candidate */

/* Power Management & Battery Monitoring */
#define AIR_BAT_ADC_PORT            GPIOA
#define AIR_BAT_ADC_PIN             GPIO_PIN_0   /* Candidate: ADC1 Channel 0 */
#define AIR_CHG_STAT_PORT           GPIOB
#define AIR_CHG_STAT_PIN            GPIO_PIN_10  /* Candidate: Input Pull-up */
#define AIR_PWR_KEY_PORT            GPIOA
#define AIR_PWR_KEY_PIN             GPIO_PIN_0   /* Candidate: WKUP / EXTI */

/* Dedicated Hardware Debug (Confirmed STM32 Pins) */
#define AIR_SWDIO_PIN               GPIO_PIN_13  /* PA13 */
#define AIR_SWCLK_PIN               GPIO_PIN_14  /* PA14 */

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
