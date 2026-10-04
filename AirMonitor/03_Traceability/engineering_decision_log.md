# Engineering Decision Log (Architectural Decision Records)

## 1. Overview
This log documents key engineering decisions, architectural trade-offs, and design rationales adopted during the development of the Air Monitor embedded system.

---

## 2. Architectural Decision Records (ADRs)

### ADR-01: Microcontroller Selection — Espressif ESP32-WROVER-B
- **Status**: ACCEPTED
- **Context**: The device requires high compute performance for real-time sensor processing, native Wi-Fi/BLE connectivity, and substantial RAM for driving a 240×320 color TFT display without memory starvation.
- **Decision**: Select the Espressif ESP32-WROVER-B module (Tensilica Xtensa dual-core LX6 @ 240 MHz with 4 MB embedded flash and 8 MB embedded PSRAM). Respect the internal SPI flash (GPIO 6–11) and PSRAM (GPIO 16–17) bus reservation constraints.
- **Consequences**: Provides ample RAM for display double-buffering, FreeRTOS multi-tasking, network TLS stacks, and offline telemetry buffering. Precludes assigning GPIO 6–11 or 16–17 to external sensors.

### ADR-02: Sensor Hardware Abstraction Layer (HAL)
- **Status**: ACCEPTED
- **Context**: Embedded air monitors must support component lifecycle management and candidate sensor evaluation without rewriting the core application.
- **Decision**: Define pure abstract C interfaces (`pm_sensor.h`, `co2_sensor.h`, `trh_sensor.h`, `display_driver.h`) isolating application logic from silicon drivers. Beneath these abstractions, implement sensor-specific drivers (e.g. `pm_sensor_pms.c` for PMS-compatible UART; `co2_sensor_scd4x.c` for Sensirion SCD4x I2C).
- **Consequences**: Preserves architectural modularity and decouples business logic from peripheral silicon. Enables straightforward qualification of second-source sensors.

### ADR-03: Plantower PMS5003 Compatible Particulate Driver
- **Status**: ACCEPTED
- **Context**: The physical enclosure accommodates a ~50×38×21mm laser dust sensor with internal centrifugal fan and 3.3V UART communications.
- **Decision**: Implement a fully functional 32-byte UART packet parser matching the Plantower PMS5003/PMS7003 frame specification (`0x42 0x4D` header, 16-bit big-endian concentration registers, checksum validation).
- **Consequences**: Provides a robust, standards-compliant parsing pipeline with strict error rejection for malformed frames.

### ADR-04: FreeRTOS Preemptive Task Architecture & Queue Decoupling
- **Status**: ACCEPTED
- **Context**: The monitor must sample sensors at precise intervals, maintain smooth 30+ FPS UI refresh, and handle asynchronous Wi-Fi/MQTT transmissions without blocking.
- **Decision**: Separate concerns into five dedicated FreeRTOS tasks communicating via thread-safe queues and event groups:
  1. `sensor_task` (Core 1, Priority 5, period 1000ms): Polls I2C/UART sensors.
  2. `pipeline_task` (Core 1, Priority 5): Applies calibration, rolling averages, and AQI computation.
  3. `ui_task` (Core 0, Priority 4, period 33ms): Renders display screens and handles touch events.
  4. `telemetry_task` (Core 0, Priority 3): Handles Wi-Fi connection, MQTT publishing, and HTTP requests.
  5. `power_task` (Core 0, Priority 1, period 5000ms): Monitors battery ADC and manages power states.
- **Consequences**: Completely eliminates network latency (DNS, TLS handshakes, retransmissions) from stalling sensor sampling or causing display frame drops.

### ADR-05: Non-Volatile Storage (NVS) & Telemetry Ring Buffer
- **Status**: ACCEPTED
- **Context**: The device requires persistence for user settings, Wi-Fi credentials, and sensor calibration constants, as well as preserving data during network outages.
- **Decision**: Utilize the ESP-IDF NVS key-value store for structured configuration parameters, coupled with a dedicated flash sector circular buffer for queuing telemetry records during Wi-Fi disconnects.
- **Consequences**: Ensures zero data loss during temporary network outages while guaranteeing wear-leveling across SPI flash sectors.

### ADR-06: Telemetry Data Model & MQTT Schema
- **Status**: ACCEPTED
- **Context**: Telemetry must easily integrate into modern smart home and industrial IoT platforms.
- **Decision**: Adopt a standardized JSON telemetry schema published to topic `devices/{device_id}/telemetry` containing timestamp, PM concentrations (PM1.0, PM2.5, PM10), CO₂, temperature, humidity, battery percentage, network RSSI, and sensor health flags.
- **Consequences**: Enables seamless integration with open-source IoT platforms (Home Assistant, Mosquitto, InfluxDB, Grafana) without proprietary protocols.

### ADR-07: Deterministic Host Simulation & Validation Layer
- **Status**: ACCEPTED
- **Context**: CI/CD pipelines, automated testing, and software integration need to run without requiring connected physical hardware on every developer workstation.
- **Decision**: Provide a standalone Python device simulator (`05_Software/device_simulator/virtual_device.py`) that emulates the ESP32 state machine, generating synthetic sensor data under defined operational scenarios (NORMAL, HIGH_PM, HIGH_CO2, SENSOR_FAULT, NETWORK_DROP).
- **Consequences**: Enables immediate, deterministic automated testing of telemetry ingestion, database storage, and dashboard visualization on standard host workstations.
