idf_component_register(
    SRCS "src/nvs_manager.c"
         "src/telemetry_buffer.c"
    INCLUDE_DIRS "include"
    REQUIRES nvs_flash freertos
)
