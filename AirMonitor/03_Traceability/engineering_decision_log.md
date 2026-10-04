# Engineering Decision Log (Architectural Decision Records)

## 1. Overview
This log documents key engineering decisions, architectural trade-offs, and design rationales adopted during the development of the Air Monitor embedded system.

---

## 2. Architectural Decision Records (ADRs)

### ADR-01: Microcontroller Selection — STMicroelectronics STM32F407ZGT6
- **Status**: ACCEPTED (Authoritative Physical Chip Identification)
- **Context**: The device requires high compute performance for real-time sensor processing, precision hardware timers, hardware floating-point acceleration for AQI calculation, and rich serial buses (USART, I2C, SPI) in a high-density package.
- **Decision**: Authoritatively establish the STMicroelectronics STM32F407ZGT6 microcontroller based on physical chip markings (ARM 32-bit Cortex-M4 with FPU @ 168 MHz, 1 MB on-chip Flash, 192 KB SRAM, LQFP-144 package).
- **Consequences**: Delivers 210 DMIPS compute performance, hardware single-precision FPU, dual DMA controllers for zero-overhead peripheral transfers, and expansive internal memory for real-time sensor filtering, display frame buffering, and system diagnostics without requiring external PSRAM.

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
- **Context**: The monitor must sample sensors at precise intervals, maintain smooth UI refresh, and handle asynchronous serial communications without blocking.
- **Decision**: Separate concerns into dedicated FreeRTOS tasks communicating via thread-safe queues:
  1. `sensor_task` (Priority 3, period 1000ms): Polls I2C/UART sensors.
  2. `pipeline_task` (Priority 3): Applies calibration, rolling averages, and AQI computation.
  3. `ui_task` (Priority 2, period 33ms): Renders display screens and handles touch events.
  4. `telemetry_task` (Priority 2): Handles serial/network telemetry output.
  5. `power_task` (Priority 1, period 5000ms): Monitors battery ADC and manages power states.
- **Consequences**: Guarantees deterministic sensor acquisition and smooth display interaction on the ARM Cortex-M4 single-core scheduler.

### ADR-05: Non-Volatile Parameter Storage & Telemetry Ring Buffer
- **Status**: ACCEPTED
- **Context**: The device requires persistence for user settings, operational thresholds, and sensor calibration constants, as well as preserving data during communication dropouts.
- **Decision**: Utilize on-chip Flash sectors / EEPROM emulation for structured configuration parameters, coupled with a circular buffer for queuing telemetry records.
- **Consequences**: Ensures zero data loss during communication outages while providing reliable configuration retention.

### ADR-06: Telemetry Data Model & Serial/MQTT Schema
- **Status**: ACCEPTED
- **Context**: Telemetry must easily integrate into modern monitoring and IoT platforms.
- **Decision**: Adopt a standardized JSON telemetry schema containing timestamp, PM concentrations (PM1.0, PM2.5, PM10), CO₂, temperature, humidity, battery percentage, and sensor health flags.
- **Consequences**: Enables seamless integration with open-source IoT platforms without proprietary protocols.

### ADR-07: Deterministic Host Simulation & Validation Layer
- **Status**: ACCEPTED
- **Context**: Automated testing and software integration need to run without requiring connected physical hardware on every developer workstation.
- **Decision**: Provide a standalone Python device simulator (`05_Software/device_simulator/virtual_device.py`) that emulates the STM32F407ZGT6 firmware state machine, generating synthetic sensor data under defined operational scenarios (NORMAL, HIGH_PM, HIGH_CO2, SENSOR_FAULT, NETWORK_DROP).
- **Consequences**: Enables immediate, deterministic automated testing of telemetry ingestion and dashboard visualization on standard host workstations.
