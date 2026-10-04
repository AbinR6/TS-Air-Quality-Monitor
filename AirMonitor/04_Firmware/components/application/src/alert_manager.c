#include "alert_manager.h"

static alert_thresholds_t s_th = {
    .pm25_warning = 35.0f,
    .pm25_critical = 75.0f,
    .co2_warning = 1000.0f,
    .co2_critical = 1500.0f,
    .hysteresis_percent = 5.0f,
};

static alert_level_t s_current_level = ALERT_LEVEL_NORMAL;

void alert_manager_init(const alert_thresholds_t *thresholds)
{
    if (thresholds != NULL) {
        s_th = *thresholds;
    }
    s_current_level = ALERT_LEVEL_NORMAL;
}

alert_level_t alert_manager_evaluate(float pm2_5, float co2_ppm)
{
    float pm_crit_th = s_th.pm25_critical;
    float co2_crit_th = s_th.co2_critical;
    float pm_warn_th = s_th.pm25_warning;
    float co2_warn_th = s_th.co2_warning;

    /* Apply downward hysteresis if currently alarmed */
    if (s_current_level == ALERT_LEVEL_CRITICAL) {
        pm_crit_th *= (1.0f - s_th.hysteresis_percent / 100.0f);
        co2_crit_th *= (1.0f - s_th.hysteresis_percent / 100.0f);
    } else if (s_current_level == ALERT_LEVEL_WARNING) {
        pm_warn_th *= (1.0f - s_th.hysteresis_percent / 100.0f);
        co2_warn_th *= (1.0f - s_th.hysteresis_percent / 100.0f);
    }

    if (pm2_5 >= pm_crit_th || co2_ppm >= co2_crit_th) {
        s_current_level = ALERT_LEVEL_CRITICAL;
    } else if (pm2_5 >= pm_warn_th || co2_ppm >= co2_warn_th) {
        s_current_level = ALERT_LEVEL_WARNING;
    } else {
        s_current_level = ALERT_LEVEL_NORMAL;
    }

    return s_current_level;
}
