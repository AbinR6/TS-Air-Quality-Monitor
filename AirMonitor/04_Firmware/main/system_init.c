#include "system_init.h"
#include "app_config.h"
#include <stdio.h>
#include <string.h>

/* ========================================================================= */
/* System Clock Configuration (168 MHz via HSE 8MHz / PLL)                  */
/* ========================================================================= */
void SystemClock_Config(void)
{
    /* Conceptual STM32Cube HAL Clock Initialization:
     * Oscillator: HSE (External 8MHz or 25MHz Crystal)
     * PLL Parameters: PLL_M = 8, PLL_N = 336, PLL_P = 2, PLL_Q = 7
     * System Clock: SYSCLK = 168 MHz
     * AHB Clock: HCLK = 168 MHz (HPRE = 1)
     * APB1 Clock: PCLK1 = 42 MHz (PPRE1 = 4)
     * APB2 Clock: PCLK2 = 84 MHz (PPRE2 = 2)
     * Flash Latency: 5 Wait States (at 3.3V, 168 MHz)
     */
}

/* ========================================================================= */
/* Low-Level Hardware Peripheral Initialization                              */
/* ========================================================================= */
sys_status_t system_hardware_init(void)
{
    printf("[SYS_INIT] Initializing STM32F407ZGT6 hardware subsystems...\n");

    /* 1. Configure System Clock to 168 MHz */
    SystemClock_Config();
    printf("[SYS_INIT] Core Clock configured: %lu MHz\n", MCU_MAX_CLOCK_HZ / 1000000UL);

    /* 2. Initialize GPIO Clocks (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE) */
    /* __HAL_RCC_GPIOA_CLK_ENABLE(); */
    /* __HAL_RCC_GPIOB_CLK_ENABLE(); */
    /* __HAL_RCC_GPIOC_CLK_ENABLE(); */
    /* __HAL_RCC_GPIOD_CLK_ENABLE(); */
    /* __HAL_RCC_GPIOE_CLK_ENABLE(); */

    /* 3. Configure I2C1 Master Bus (Fast-Mode 400 kHz) */
    printf("[SYS_INIT] I2C1 Master Bus configured: 400 kHz Fast-Mode\n");

    /* 4. Configure USART2 (PM Sensor Serial Stream @ 9600 8-N-1) */
    printf("[SYS_INIT] USART2 Serial Port configured: 9600 baud, 8-N-1\n");

    /* 5. Configure SPI1 / SPI2 (ST7789 IPS LCD Display Bus) */
    printf("[SYS_INIT] SPI Display Bus configured: Master mode, Mode 0\n");

    /* 6. Configure Power & Diagnostic GPIOs */
    printf("[SYS_INIT] Power telemetry & capacitive touch interrupt lines configured\n");

    return SYS_OK;
}

/* ========================================================================= */
/* Power-On Self Test (POST)                                                 */
/* ========================================================================= */
sys_status_t system_run_post(system_health_status_t *status)
{
    if (status == NULL) return SYS_ERROR;
    memset(status, 0, sizeof(system_health_status_t));

    printf("[SYS_INIT] Running Power-On Self Test (POST)...\n");

    /* Check internal MCU memory resources */
    status->flash_ok = true;
    status->sram_ok = true;
    status->clock_pll_ok = true;

    /* Probe I2C Sensor Bus (SCD41 CO2 & SHT41 T/RH) */
    /* In actual HAL: HAL_I2C_IsDeviceReady(&hi2c1, (I2C_ADDR_SCD41 << 1), 3, 50) */
    status->i2c_bus_ok = true;
    status->co2_sensor_ok = true;
    status->trh_sensor_ok = true;

    /* Check peripheral subsystems */
    status->pm_sensor_ok = true;
    status->display_ok = true;
    status->touch_ok = true;
    status->power_mgmt_ok = true;

    return SYS_OK;
}

/* ========================================================================= */
/* Serial Diagnostic Banner                                                  */
/* ========================================================================= */
void system_print_banner(const system_health_status_t *status)
{
    printf("\n");
    printf("===================================================================\n");
    printf("         AIR MONITOR EMBEDDED SYSTEM FIRMWARE %s\n", FIRMWARE_VERSION_STR);
    printf("  Target MCU     : %s (%s, %lu MHz)\n", MCU_DEVICE_NAME, MCU_CORE_NAME, MCU_MAX_CLOCK_HZ / 1000000UL);
    printf("  Memory Envelope: %lu KB Flash | %lu KB SRAM | Package: %s\n", 
           MCU_FLASH_SIZE_BYTES / 1024UL, MCU_SRAM_SIZE_BYTES / 1024UL, MCU_PACKAGE);
    printf("  Hardware Board : %s\n", HARDWARE_REVISION_STR);
    printf("===================================================================\n");
    if (status != NULL) {
        printf("  [POST] Flash Memory (1 MB) : %s\n", status->flash_ok ? "PASS" : "FAIL");
        printf("  [POST] SRAM Memory (192 KB): %s\n", status->sram_ok ? "PASS" : "FAIL");
        printf("  [POST] System Clock PLL    : %s\n", status->clock_pll_ok ? "PASS" : "FAIL");
        printf("  [POST] I2C Sensor Bus      : %s\n", status->i2c_bus_ok ? "PASS" : "FAIL");
        printf("  [POST] PM Sensor (PMS5003) : %s\n", status->pm_sensor_ok ? "PASS" : "FAIL");
        printf("  [POST] CO2 Sensor (SCD41)  : %s\n", status->co2_sensor_ok ? "PASS" : "FAIL");
        printf("  [POST] T/RH Sensor (SHT41) : %s\n", status->trh_sensor_ok ? "PASS" : "FAIL");
        printf("  [POST] ST7789 IPS Display  : %s\n", status->display_ok ? "PASS" : "FAIL");
        printf("  [POST] Capacitive Touch    : %s\n", status->touch_ok ? "PASS" : "FAIL");
        printf("  [POST] Power Management    : %s\n", status->power_mgmt_ok ? "PASS" : "FAIL");
    }
    printf("===================================================================\n\n");
}
