# Firmware Architecture & Implementation Specification

## 1. Overview
The firmware is developed in Embedded C targeting the STM32F407ZGT6 high-performance ARM Cortex-M4 microcontroller using the STM32 HAL (Hardware Abstraction Layer) and STM32Cube framework. It implements a preemptive multi-threaded architecture with hardware drivers, application algorithms, UI rendering, local storage, and communication stacks.

---

## 2. Directory Layout & Component Modularization
- `04_Firmware/main/`: Core application lifecycle, task spawning, system clock configuration, and POST.
- `04_Firmware/components/drivers/`: Low-level peripheral drivers:
  - `pm_sensor_pms.c`: Plantower PMS UART packet decoder with checksum verification.
  - `co2_sensor_scd4x.c`: Sensirion SCD4x I2C driver with CRC8 checking.
  - `trh_sensor_sht4x.c`: Sensirion SHT4x precision T/RH driver.
  - `display_st7789.c`: SPI TFT display driver with double-buffering.
  - `touch_cst816.c`: Capacitive touch controller driver.
  - `power_driver.c`: Battery ADC and charging status GPIO driver.
- `04_Firmware/components/application/`:
  - `measurement_pipeline.c`: Exponential moving average smoothing and outlier rejection.
  - `air_quality_index.c`: US EPA AQI calculation engine.
  - `alert_manager.c`: Hysteresis-based threshold alerting.
- `04_Firmware/components/display/`:
  - `ui_manager.c`: Screen navigation state machine.
  - `ui_screens.c`: Rendering routines for dashboard, PM detail, CO2 detail, and diagnostics.
  - `ui_theme.c`: RGB565 color palette and typography tokens.
- `04_Firmware/components/network/`:
  - `wifi_manager.c`: Network interface manager with auto-reconnect logic.
  - `mqtt_manager.c`: Telemetry publisher to `devices/{id}/telemetry`.
  - `http_server.c`: REST service handler for local discovery.
  - `telemetry_encoder.c`: JSON telemetry serialization.
- `04_Firmware/components/storage/`:
  - `nvs_manager.c`: Key-value configuration storage in non-volatile flash.
  - `telemetry_buffer.c`: Ring buffer for caching telemetry during offline intervals.
- `04_Firmware/components/configuration/`:
  - `device_config.c`: Factory defaults and runtime profile management.
- `04_Firmware/components/calibration/`:
  - `calibration_manager.c`: Zero-offset and multi-point span calibration math.
- `04_Firmware/components/diagnostics/`:
  - `self_test.c`: Memory watermark and peripheral self-test routines.
  - `fault_logger.c`: Circular fault event logger.
- `04_Firmware/components/power/`:
  - `power_manager.c`: Battery state-of-charge calculation and sleep control.

---

## 3. Task Synchronization & Concurrency
Tasks communicate via thread-safe queues and mutexes. Sensor polling executes at Priority 5, while UI rendering (Priority 4) and communication/storage (Priority 3) execute with dedicated prioritization on the 168 MHz Cortex-M4 core. This ensures that communication handshakes or storage flushes never cause UI frame drops or miss real-time sensor polling windows.