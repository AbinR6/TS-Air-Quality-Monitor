idf_component_register(
    SRCS "src/pm_sensor_pms.c"
         "src/co2_sensor_scd4x.c"
         "src/trh_sensor_sht4x.c"
         "src/display_st7789.c"
         "src/touch_cst816.c"
    INCLUDE_DIRS "include"
    REQUIRES driver esp_timer freertos
)
