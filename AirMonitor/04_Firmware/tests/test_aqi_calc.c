#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../components/application/include/air_quality_index.h"

void test_aqi_breakpoints(void)
{
    printf("[TEST] Running test_aqi_breakpoints...\n");

    /* Test 0.0 ug/m3 -> AQI 0 */
    assert(aqi_calculate_pm2_5(0.0f) == 0);

    /* Test 12.0 ug/m3 -> AQI 50 (Good boundary) */
    assert(aqi_calculate_pm2_5(12.0f) == 50);

    /* Test 35.4 ug/m3 -> AQI 100 (Moderate boundary) */
    assert(aqi_calculate_pm2_5(35.4f) == 100);

    /* Test 55.4 ug/m3 -> AQI 150 (Unhealthy for Sensitive Groups boundary) */
    assert(aqi_calculate_pm2_5(55.4f) == 150);

    /* Test 150.4 ug/m3 -> AQI 200 (Unhealthy boundary) */
    assert(aqi_calculate_pm2_5(150.4f) == 200);

    /* Test Categories */
    assert(aqi_get_category(25) == AQI_CAT_GOOD);
    assert(aqi_get_category(75) == AQI_CAT_MODERATE);
    assert(aqi_get_category(125) == AQI_CAT_UNHEALTHY_SENSITIVE);
    assert(aqi_get_category(175) == AQI_CAT_UNHEALTHY);
    assert(aqi_get_category(250) == AQI_CAT_VERY_UNHEALTHY);
    assert(aqi_get_category(350) == AQI_CAT_HAZARDOUS);

    printf("[TEST] test_aqi_breakpoints: PASS\n");
}

int main(void)
{
    test_aqi_breakpoints();
    printf("[ALL TESTS PASSED]\n");
    return 0;
}
