idf_component_register(
    SRCS "main.c" "system_init.c"
    INCLUDE_DIRS "."
    REQUIRES drivers application display network storage configuration calibration diagnostics power esp_timer nvs_flash esp_wifi esp_event esp_netif mqtt
)
