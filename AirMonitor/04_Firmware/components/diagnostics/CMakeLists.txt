idf_component_register(
    SRCS "src/self_test.c"
         "src/fault_logger.c"
    INCLUDE_DIRS "include"
    REQUIRES drivers freertos
)
