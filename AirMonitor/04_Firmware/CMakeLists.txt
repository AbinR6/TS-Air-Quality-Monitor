cmake_minimum_required(VERSION 3.16)
project(air_monitor_stm32 C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Microcontroller Target Definition
set(MCU_FAMILY "STM32F4xx")
set(MCU_DEVICE "STM32F407xx")
set(MCU_PART "STM32F407ZGT6")

# Include directories
include_directories(
    main
    components/application/include
    components/calibration/include
    components/configuration/include
    components/diagnostics/include
    components/display/include
    components/drivers/include
    components/network/include
    components/power/include
    components/storage/include
)

# Source files
file(GLOB_RECURSE SOURCES
    main/*.c
    components/application/src/*.c
    components/calibration/src/*.c
    components/configuration/src/*.c
    components/diagnostics/src/*.c
    components/display/src/*.c
    components/drivers/src/*.c
    components/network/src/*.c
    components/power/src/*.c
    components/storage/src/*.c
)

add_definitions(-DSTM32F407xx -DUSE_HAL_DRIVER)

add_library(${PROJECT_NAME}_lib STATIC ${SOURCES})
