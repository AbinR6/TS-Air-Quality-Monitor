#include "measurement_pipeline.h"
#include "air_quality_index.h"
#include <string.h>

#define EMA_ALPHA   0.25f  /* Exponential Moving Average smoothing factor */

static struct {
    float pm1_ema;
    float pm25_ema;
    float pm10_ema;
    float co2_ema;
    float temp_ema;
    float rh_ema;
    bool initialized;
} s_filter_state;

esp_err_t measurement_pipeline_init(void)
{
    memset(&s_filter_state, 0, sizeof(s_filter_state));
    s_filter_state.initialized = false;
    return ESP_OK;
}

esp_err_t measurement_pipeline_process(float raw_pm1, float raw_pm25, float raw_pm10,
                                       float raw_co2, float raw_temp, float raw_rh,
                                       processed_metrics_t *output)
{
    if (output == NULL) return ESP_ERR_INVALID_ARG;

    /* Validity Bounds Checking */
    output->pm_valid  = (raw_pm25 >= 0.0f && raw_pm25 <= 1000.0f);
    output->co2_valid = (raw_co2 >= 350.0f && raw_co2 <= 10000.0f);
    output->trh_valid = (raw_temp >= -20.0f && raw_temp <= 85.0f && raw_rh >= 0.0f && raw_rh <= 100.0f);

    if (!s_filter_state.initialized) {
        s_filter_state.pm1_ema  = raw_pm1;
        s_filter_state.pm25_ema = raw_pm25;
        s_filter_state.pm10_ema = raw_pm10;
        s_filter_state.co2_ema  = raw_co2;
        s_filter_state.temp_ema = raw_temp;
        s_filter_state.rh_ema   = raw_rh;
        s_filter_state.initialized = true;
    } else {
        if (output->pm_valid) {
            s_filter_state.pm1_ema  = EMA_ALPHA * raw_pm1  + (1.0f - EMA_ALPHA) * s_filter_state.pm1_ema;
            s_filter_state.pm25_ema = EMA_ALPHA * raw_pm25 + (1.0f - EMA_ALPHA) * s_filter_state.pm25_ema;
            s_filter_state.pm10_ema = EMA_ALPHA * raw_pm10 + (1.0f - EMA_ALPHA) * s_filter_state.pm10_ema;
        }
        if (output->co2_valid) {
            s_filter_state.co2_ema  = EMA_ALPHA * raw_co2  + (1.0f - EMA_ALPHA) * s_filter_state.co2_ema;
        }
        if (output->trh_valid) {
            s_filter_state.temp_ema = EMA_ALPHA * raw_temp + (1.0f - EMA_ALPHA) * s_filter_state.temp_ema;
            s_filter_state.rh_ema   = EMA_ALPHA * raw_rh   + (1.0f - EMA_ALPHA) * s_filter_state.rh_ema;
        }
    }

    output->pm1_0         = s_filter_state.pm1_ema;
    output->pm2_5         = s_filter_state.pm25_ema;
    output->pm10          = s_filter_state.pm10_ema;
    output->co2_ppm       = s_filter_state.co2_ema;
    output->temperature_c = s_filter_state.temp_ema;
    output->humidity_rh   = s_filter_state.rh_ema;
    output->aqi           = aqi_calculate_pm2_5(output->pm2_5);

    return ESP_OK;
}
