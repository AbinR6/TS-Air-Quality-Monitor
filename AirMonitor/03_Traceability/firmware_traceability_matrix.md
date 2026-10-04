# Firmware & Software Traceability Matrix

## 1. Overview
This matrix traces every firmware module, hardware driver, FreeRTOS task, and application service to hardware pin contracts, operational requirements, and automated validation tests.

---

## 2. Firmware Module Traceability Table

| Module Name | Path | Hardware Interface / Contract | FreeRTOS Task / Priority | Operational Requirement | Unit Test / Verification File | Status |
|:---|:---|:---|:---|:---|:---|:---|
| **System Init** | `main/system_init.c` | STM32F407 Clock, Power, Flash, GPIO | Main / Sys Init | System startup, peripheral POST, banner | `main/system_init.c` (POST check) | VERIFIED (Structurally) |
| **PMS Driver** | `components/drivers/src/pm_sensor_pms.c` | USART2 (Candidate: PA2/PA3 or PD5/PD6) | Sensor Task (Pri 3) | `REQ-ENV-01` (PM1.0, PM2.5, PM10) | `tests/test_pms_parser.c` | VERIFIED (Structurally) |
| **SCD4x Driver** | `components/drivers/src/co2_sensor_scd4x.c` | I2C1 (Candidate: PB6/PB7, addr `0x62`) | Sensor Task (Pri 3) | `REQ-ENV-02` (CO2, Temp, RH) | `tests/test_co2_driver.c` | VERIFIED (Structurally) |
| **SHT4x Driver** | `components/drivers/src/trh_sensor_sht4x.c` | I2C1 (Candidate: PB6/PB7, addr `0x44`) | Sensor Task (Pri 3) | `REQ-ENV-03` (Precision T/RH) | `tests/test_trh_driver.c` | VERIFIED (Structurally) |
| **ST7789 Driver**| `components/drivers/src/display_st7789.c` | SPI1/SPI2 (Candidate: PA5/PA7 or PB13/PB15) | UI Task (Pri 2) | `REQ-HW-04` (240x320 IPS Display) | `tests/test_display_driver.c` | VERIFIED (Structurally) |
| **CST816 Driver**| `components/drivers/src/touch_cst816.c` | I2C1 / EXTI (TOUCH_INT) | UI / Event Task (Pri 2) | `REQ-HW-05` (Capacitive Touch Slider) | `tests/test_touch_driver.c` | VERIFIED (Structurally) |
| **Measurement Pipeline** | `components/application/src/measurement_pipeline.c`| Driver abstraction layers | Pipeline Task (Pri 3) | Acquisition, rolling average, validation | `tests/test_measurement_pipeline.c`| VERIFIED (Structurally) |
| **AQI Engine** | `components/application/src/air_quality_index.c` | Memory data structures | Synchronous library | `REQ-ENV-04` (EPA/WHO AQI standards) | `tests/test_aqi_calc.c` | VERIFIED (Structurally) |
| **UI Manager** | `components/display/src/ui_manager.c` | Display Driver & Event Queue | UI Task (Pri 2) | Screen state machine (DASH, PM, CO2) | `tests/test_ui_manager.c` | VERIFIED (Structurally) |
| **Network Manager**| `components/network/src/wifi_manager.c` | Serial / Network Co-processor Interface | Telemetry Task (Pri 2) | `REQ-NET-01` (Wireless network communication) | `tests/test_wifi_state.c` | VERIFIED (Structurally) |
| **MQTT Client** | `components/network/src/mqtt_manager.c` | Telemetry Serial / IP client | Telemetry Task (Pri 2) | `REQ-NET-02` (JSON telemetry publishing) | `tests/test_mqtt_encoder.c` | VERIFIED (Structurally) |
| **HTTP Server** | `components/network/src/http_server.c` | Embedded Web / Diagnostic Server | Telemetry Task (Pri 1) | `REQ-NET-03` (Local metrics REST API) | `tests/test_http_api.c` | VERIFIED (Structurally) |
| **Flash Storage**| `components/storage/src/nvs_manager.c` | On-chip Flash Sector / EEPROM emulation | Synchronous API | Device settings, calibration, thresholds | `tests/test_nvs_storage.c` | VERIFIED (Structurally) |
| **Buffer Cache**| `components/storage/src/telemetry_buffer.c`| Flash sector ring buffer | Storage Task (Pri 1) | `REQ-NET-04` (Offline telemetry buffering) | `tests/test_ring_buffer.c` | VERIFIED (Structurally) |
| **Power Manager**| `components/power/src/power_manager.c` | ADC1 (BAT_SENSE), GPIO (CHG_STAT) | Power Task (Pri 1) | `REQ-HW-02` (Battery state & sleep control) | `tests/test_power_manager.c` | VERIFIED (Structurally) |
