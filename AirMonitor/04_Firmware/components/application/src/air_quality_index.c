#include "air_quality_index.h"
#include <math.h>

/* US EPA AQI Breakpoints for PM2.5 (ug/m3) */
static const struct {
    float c_low;
    float c_high;
    uint16_t i_low;
    uint16_t i_high;
} PM25_BREAKPOINTS[] = {
    {  0.0f,  12.0f,   0,  50 },
    { 12.1f,  35.4f,  51, 100 },
    { 35.5f,  55.4f, 101, 150 },
    { 55.5f, 150.4f, 151, 200 },
    { 150.5f, 250.4f, 201, 300 },
    { 250.5f, 500.4f, 301, 500 },
};

#define NUM_PM25_BP (sizeof(PM25_BREAKPOINTS) / sizeof(PM25_BREAKPOINTS[0]))

uint16_t aqi_calculate_pm2_5(float c)
{
    if (c < 0.0f) return 0;
    if (c > 500.4f) return 500;

    for (int i = 0; i < NUM_PM25_BP; i++) {
        if (c <= PM25_BREAKPOINTS[i].c_high) {
            float c_low  = PM25_BREAKPOINTS[i].c_low;
            float c_high = PM25_BREAKPOINTS[i].c_high;
            uint16_t i_low  = PM25_BREAKPOINTS[i].i_low;
            uint16_t i_high = PM25_BREAKPOINTS[i].i_high;

            float aqi = ((float)(i_high - i_low) / (c_high - c_low)) * (c - c_low) + (float)i_low;
            return (uint16_t)roundf(aqi);
        }
    }
    return 500;
}

uint16_t aqi_calculate_pm10(float pm10)
{
    if (pm10 < 0.0f) return 0;
    if (pm10 <= 54.0f)  return (uint16_t)((50.0f / 54.0f) * pm10);
    if (pm10 <= 154.0f) return (uint16_t)(((100.0f - 51.0f) / (154.0f - 55.0f)) * (pm10 - 55.0f) + 51.0f);
    if (pm10 <= 254.0f) return (uint16_t)(((150.0f - 101.0f) / (254.0f - 155.0f)) * (pm10 - 155.0f) + 101.0f);
    if (pm10 <= 354.0f) return (uint16_t)(((200.0f - 151.0f) / (354.0f - 255.0f)) * (pm10 - 255.0f) + 151.0f);
    if (pm10 <= 424.0f) return (uint16_t)(((300.0f - 201.0f) / (424.0f - 355.0f)) * (pm10 - 355.0f) + 201.0f);
    return 500;
}

aqi_category_t aqi_get_category(uint16_t aqi)
{
    if (aqi <= 50)  return AQI_CAT_GOOD;
    if (aqi <= 100) return AQI_CAT_MODERATE;
    if (aqi <= 150) return AQI_CAT_UNHEALTHY_SENSITIVE;
    if (aqi <= 200) return AQI_CAT_UNHEALTHY;
    if (aqi <= 300) return AQI_CAT_VERY_UNHEALTHY;
    return AQI_CAT_HAZARDOUS;
}

uint16_t aqi_get_color_rgb565(aqi_category_t category)
{
    switch (category) {
        case AQI_CAT_GOOD:                return 0x07E0; /* Bright Green */
        case AQI_CAT_MODERATE:            return 0xFFE0; /* Yellow */
        case AQI_CAT_UNHEALTHY_SENSITIVE: return 0xFD20; /* Orange */
        case AQI_CAT_UNHEALTHY:           return 0xF800; /* Red */
        case AQI_CAT_VERY_UNHEALTHY:      return 0x780F; /* Purple */
        case AQI_CAT_HAZARDOUS:           return 0x8000; /* Maroon */
        default:                          return 0xFFFF; /* White */
    }
}

const char *aqi_get_category_str(aqi_category_t category)
{
    switch (category) {
        case AQI_CAT_GOOD:                return "Good";
        case AQI_CAT_MODERATE:            return "Moderate";
        case AQI_CAT_UNHEALTHY_SENSITIVE: return "Unhealthy for Sensitive";
        case AQI_CAT_UNHEALTHY:           return "Unhealthy";
        case AQI_CAT_VERY_UNHEALTHY:      return "Very Unhealthy";
        case AQI_CAT_HAZARDOUS:           return "Hazardous";
        default:                          return "Unknown";
    }
}
