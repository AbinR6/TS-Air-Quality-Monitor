idf_component_register(
    SRCS "src/measurement_pipeline.c"
         "src/air_quality_index.c"
         "src/alert_manager.c"
    INCLUDE_DIRS "include"
    REQUIRES drivers freertos
)
