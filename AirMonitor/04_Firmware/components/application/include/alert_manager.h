#ifndef ALERT_MANAGER_H
#define ALERT_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float pm25_warning;
    float pm25_critical;
    float co2_warning;
    float co2_critical;
    float hysteresis_percent;
} alert_thresholds_t;

typedef enum {
    ALERT_LEVEL_NORMAL = 0,
    ALERT_LEVEL_WARNING,
    ALERT_LEVEL_CRITICAL
} alert_level_t;

/**
 * @brief Initialize alert manager with threshold limits.
 */
void alert_manager_init(const alert_thresholds_t *thresholds);

/**
 * @brief Evaluate metrics against thresholds and return active alert state.
 */
alert_level_t alert_manager_evaluate(float pm2_5, float co2_ppm);

#ifdef __cplusplus
}
#endif

#endif /* ALERT_MANAGER_H */
