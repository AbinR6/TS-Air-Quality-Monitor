#ifndef AIR_QUALITY_INDEX_H
#define AIR_QUALITY_INDEX_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    AQI_CAT_GOOD = 0,               /* 0 - 50 (Green) */
    AQI_CAT_MODERATE,               /* 51 - 100 (Yellow) */
    AQI_CAT_UNHEALTHY_SENSITIVE,    /* 101 - 150 (Orange) */
    AQI_CAT_UNHEALTHY,              /* 151 - 200 (Red) */
    AQI_CAT_VERY_UNHEALTHY,         /* 201 - 300 (Purple) */
    AQI_CAT_HAZARDOUS               /* 301 - 500 (Maroon) */
} aqi_category_t;

/**
 * @brief Calculate US EPA Air Quality Index (AQI) based on PM2.5 concentration [ug/m3].
 * @param[in] pm2_5_ug_m3 24-hour or instantaneous PM2.5 concentration.
 * @return AQI integer score (0 to 500).
 */
uint16_t aqi_calculate_pm2_5(float pm2_5_ug_m3);

/**
 * @brief Calculate US EPA Air Quality Index (AQI) based on PM10 concentration [ug/m3].
 * @param[in] pm10_ug_m3 PM10 concentration.
 * @return AQI integer score (0 to 500).
 */
uint16_t aqi_calculate_pm10(float pm10_ug_m3);

/**
 * @brief Determine AQI category classification.
 * @param[in] aqi Numeric AQI value (0 to 500).
 */
aqi_category_t aqi_get_category(uint16_t aqi);

/**
 * @brief Return 16-bit RGB565 color corresponding to AQI category.
 */
uint16_t aqi_get_color_rgb565(aqi_category_t category);

/**
 * @brief Return string representation of category name.
 */
const char *aqi_get_category_str(aqi_category_t category);

#ifdef __cplusplus
}
#endif

#endif /* AIR_QUALITY_INDEX_H */
