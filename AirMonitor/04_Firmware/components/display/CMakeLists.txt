idf_component_register(
    SRCS "src/ui_manager.c"
         "src/ui_screens.c"
         "src/ui_theme.c"
    INCLUDE_DIRS "include"
    REQUIRES drivers application freertos
)
