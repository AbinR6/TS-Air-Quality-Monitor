# Requirements Traceability Matrix

## 1. Environmental Sensing Requirements

| Req ID | Description | Source / Rationale | Target Performance | Implementing Subsystem | Verification Method | Status |
|:---|:---|:---|:---|:---|:---|:---|
| **REQ-ENV-01** | Measure particulate matter PM1.0, PM2.5, PM10 | Core air monitor function | Range 0–1000 µg/m³, resolution 1 µg/m³ | `components/drivers/src/pm_sensor_pms.c` | Host parser unit tests + simulation | VERIFIED (Structurally) |
| **REQ-ENV-02** | Measure Carbon Dioxide (CO2) | Indoor air quality indicator | Range 400–5000 ppm, accuracy ±(40ppm+5%) | `components/drivers/src/co2_sensor_scd4x.c` | Driver mock tests + I2C CRC tests | VERIFIED (Structurally) |
| **REQ-ENV-03** | Measure ambient Temperature and Relative Humidity | Environmental comfort & sensor compensation | Temp: -10°C to +60°C (±0.2°C); RH: 0–100% | `components/drivers/src/trh_sensor_sht4x.c` | Unit tests on polynomial conversion | VERIFIED (Structurally) |
| **REQ-ENV-04** | Air Quality Index (AQI) Calculation | User-facing air quality health classification | US EPA AQI standard breakpoints (0–500) | `components/application/src/air_quality_index.c` | Algorithmic test vectors | VERIFIED (Structurally) |

---

## 2. Hardware & Electrical Requirements

| Req ID | Description | Source / Rationale | Target Specification | Implementing Hardware | Verification Method | Status |
|:---|:---|:---|:---|:---|:---|:---|
| **REQ-HW-01** | Core Microcontroller Platform | E001 marking | Espressif ESP32-WROVER-B (240MHz, 8MB PSRAM)| `02_Hardware/kicad/air_monitor.kicad_sch` | Static pin/bus audit | VERIFIED (Structurally) |
| **REQ-HW-02** | Battery Power & Portability | E003 battery observation | Single-cell 18650 Li-ion, >8 hours run time | `02_Hardware/power_tree.md`, `battery_monitor.c` | Power budget analysis | VERIFIED (Structurally) |
| **REQ-HW-03** | USB-C External Charging & Flashing | E001/E003 port | 5V USB-C charging @ 500mA + UART flashing | `02_Hardware/kicad/air_monitor.kicad_sch` | Schematic review | VERIFIED (Structurally) |
| **REQ-HW-04** | Display Visual Interface | E001 (J1), E004 | 2.1" 240×320 IPS LCD via 4-wire SPI (40MHz)| `components/drivers/src/display_st7789.c` | Framebuffer & SPI timing review | VERIFIED (Structurally) |
| **REQ-HW-05** | Top Capacitive Touch Navigation | E001 (J2), E003 | Top slider with tap, swipe, long-press | `components/drivers/src/touch_cst816.c` | State machine unit tests | VERIFIED (Structurally) |

---

## 3. Connectivity & Telemetry Requirements

| Req ID | Description | Source / Rationale | Target Specification | Implementing Subsystem | Verification Method | Status |
|:---|:---|:---|:---|:---|:---|:---|
| **REQ-NET-01** | Wi-Fi 802.11 b/g/n Connectivity | Remote telemetry upload | WPA2-Personal, station mode, auto-reconnect | `components/network/src/wifi_manager.c` | Connection state machine test | VERIFIED (Structurally) |
| **REQ-NET-02** | MQTT Telemetry Publishing | IoT platform ingestion | JSON telemetry payload, QoS 1, periodic 60s | `components/network/src/mqtt_manager.c` | Schema validation & packet analyzer | VERIFIED (Structurally) |
| **REQ-NET-03** | Local REST HTTP API | Local network discovery | GET `/api/v1/metrics`, GET `/api/v1/status` | `components/network/src/http_server.c` | Endpoint mock tests | VERIFIED (Structurally) |
| **REQ-NET-04** | Offline Telemetry Caching | Resilience against network dropouts | Ring buffer in NVS/Flash, capacity >500 records | `components/storage/src/telemetry_buffer.c` | FIFO circular buffer tests | VERIFIED (Structurally) |
