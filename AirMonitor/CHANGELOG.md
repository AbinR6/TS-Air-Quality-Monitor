# Changelog

All notable changes to the Air Monitor engineering project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.0.0] — 2026-10-04

### Added
- **Hardware Design**:
  - Complete 4-layer KiCad schematic and PCB layout for the 64 mm × 64 mm main board.
  - Comprehensive Bill of Materials (BOM) in CSV and Markdown formats.
  - Complete power tree architecture and pin assignment matrix for ESP32-WROVER-B.
  - High-precision thermal decoupling cutout for the SHT41 climate sensor.
- **Firmware (ESP-IDF v5.3.2)**:
  - Multi-tasking FreeRTOS architecture with core affinity separation.
  - Hardware drivers: Plantower PMS5003 UART, Sensirion SCD41 I2C, Sensirion SHT41 I2C, ST7789 40MHz SPI display, and CST816S capacitive touch.
  - US EPA Air Quality Index (AQI) calculation engine and exponential moving average filter.
  - Graphical UI carousel: Boot, Main Dashboard, PM Detail, CO₂ Detail, Climate, and Diagnostics.
  - 802.11 b/g/n Wi-Fi manager with exponential backoff and auto-reconnect.
  - MQTT telemetry client (QoS 1) and local REST HTTP server (`/api/v1/metrics`, `/api/v1/status`).
  - NVS flash key-value storage and offline circular telemetry ring buffer.
- **Software & Host Tools**:
  - Python-based virtual device simulator supporting 5 operational scenarios.
  - Telemetry receiver daemon with SQLite relational logging.
  - Packet schema analyzer and device provisioning utility.
- **Mechanical & 3D CAD**:
  - Mechanical enclosure CAD specification and 3D STL assets for PCB, display, sensor, and MCU.
- **Documentation & Traceability**:
  - Comprehensive engineering documentation in `07_Documentation/`.
  - Requirements, hardware, firmware, software, and validation traceability matrices.

---

## [0.2.0] — 2026-09-20

### Added
- Initial KiCad schematic capture and component symbol mapping.
- Power distribution network sizing and battery charging controller selection.
- FreeRTOS task hierarchy definition and queue communication model.

---

## [0.1.0] — 2026-09-01

### Added
- Project inception and system architecture specification.
- Environmental sensor evaluation and requirements traceability framework.
