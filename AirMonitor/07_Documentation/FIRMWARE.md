# Firmware Architecture & Implementation Specification

## 1. Overview
The firmware is developed under the Espressif IoT Development Framework (ESP-IDF v5.3.2) in ANSI C. It implements a preemptive multi-threaded architecture with hardware drivers, application algorithms, UI rendering, local storage, and networking stacks.

---

## 2. Directory Layout & Component Modularization
- `04_Firmware/main/`: Core application lifecycle, FreeRTOS task spawning, and system POST.
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
  - `wifi_manager.c`: Wi-Fi Station connection manager with auto-reconnect.
  - `mqtt_manager.c`: MQTT client publishing telemetry to `devices/{id}/telemetry`.
  - `http_server.c`: REST HTTP server for local network discovery.
  - `telemetry_encoder.c`: JSON telemetry serialization.
- `04_Firmware/components/storage/`:
  - `nvs_manager.c`: Key-value configuration storage in NVS flash.
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

## 3. FreeRTOS Task Synchronization
All tasks communicate via queues and mutexes. Sensor polling runs on Core 1 at Priority 5, while UI rendering (Priority 4) and network communications (Priority 3) run on Core 0. This ensures that network stalls or Wi-Fi reconnect handshakes never cause UI frame drops or miss sensor polling windows.