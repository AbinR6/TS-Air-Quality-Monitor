#ifndef SYSTEM_INIT_H
#define SYSTEM_INIT_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* System Status Return Codes */
typedef enum {
    SYS_OK       = 0x00,
    SYS_ERROR    = 0x01,
    SYS_BUSY     = 0x02,
    SYS_TIMEOUT  = 0x03
} sys_status_t;

/* System Power-On Self Test Health Metrics */
typedef struct {
    bool flash_ok;
    bool sram_ok;
    bool clock_pll_ok;
    bool i2c_bus_ok;
    bool pm_sensor_ok;
    bool co2_sensor_ok;
    bool trh_sensor_ok;
    bool display_ok;
    bool touch_ok;
    bool power_mgmt_ok;
} system_health_status_t;

/**
 * @brief Configure STM32F407ZGT6 system clocks (168 MHz via HSE PLL).
 */
void SystemClock_Config(void);

/**
 * @brief Initialize low-level STM32 HAL peripherals, clocks, memory, and buses.
 * @return SYS_OK on success, or error code.
 */
sys_status_t system_hardware_init(void);

/**
 * @brief Execute Power-On Self Test (POST) and populate health metrics.
 * @param[out] status Pointer to health status structure.
 * @return SYS_OK if minimum boot prerequisites pass.
 */
sys_status_t system_run_post(system_health_status_t *status);

/**
 * @brief Print boot diagnostic banner to serial console (USART).
 */
void system_print_banner(const system_health_status_t *status);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_INIT_H */
