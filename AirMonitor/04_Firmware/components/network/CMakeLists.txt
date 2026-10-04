idf_component_register(
    SRCS "src/wifi_manager.c"
         "src/mqtt_manager.c"
         "src/http_server.c"
         "src/telemetry_encoder.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_wifi esp_event esp_netif mqtt application freertos
)
