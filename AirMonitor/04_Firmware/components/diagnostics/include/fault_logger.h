#ifndef FAULT_LOGGER_H
#define FAULT_LOGGER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    FAULT_NONE = 0,
    FAULT_I2C_BUS_TIMEOUT,
    FAULT_PM_SENSOR_CHECKSUM,
    FAULT_CO2_SENSOR_COMM,
    FAULT_WIFI_DISCONNECT,
    FAULT_MQTT_PUBLISH_FAIL,
    FAULT_BATTERY_UNDERVOLT,
    FAULT_PSRAM_ALLOC_FAIL
} system_fault_code_t;

/**
 * @brief Record a system fault event into circular diagnostic log.
 */
void fault_logger_record(system_fault_code_t code);

/**
 * @brief Get total fault count.
 */
uint32_t fault_logger_get_count(void);

#ifdef __cplusplus
}
#endif

#endif /* FAULT_LOGGER_H */
