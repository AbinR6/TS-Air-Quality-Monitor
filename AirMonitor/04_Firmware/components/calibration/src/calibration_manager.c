#include "calibration_manager.h"
#include "esp_log.h"

static const char *TAG = "CALIB_MGR";

static sensor_calibration_t s_calib = {
    .pm25_slope = 1.0f,
    .pm25_offset = 0.0f,
    .co2_slope = 1.0f,
    .co2_offset = 0.0f,
    .temp_offset = -0.5f, /* Board heat conduction compensation */
    .rh_offset = 1.2f,
};

esp_err_t calibration_manager_init(void)
{
    ESP_LOGI(TAG, "Initializing sensor calibration subsystem...");
    return ESP_OK;
}

float calibration_apply_pm25(float raw_pm25)
{
    float val = (raw_pm25 * s_calib.pm25_slope) + s_calib.pm25_offset;
    return (val < 0.0f) ? 0.0f : val;
}

float calibration_apply_co2(float raw_co2)
{
    float val = (raw_co2 * s_calib.co2_slope) + s_calib.co2_offset;
    return (val < 400.0f) ? 400.0f : val;
}

float calibration_apply_temp(float raw_temp)
{
    return raw_temp + s_calib.temp_offset;
}

float calibration_apply_rh(float raw_rh)
{
    float val = raw_rh + s_calib.rh_offset;
    if (val < 0.0f) return 0.0f;
    if (val > 100.0f) return 100.0f;
    return val;
}
