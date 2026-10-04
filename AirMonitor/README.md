# Air Monitor

An embedded environmental monitoring system built around the STM32F407ZGT6 microcontroller, integrating particulate, CO₂, temperature/humidity sensing, display interaction, power management, and telemetry.

---

## 1. Project Overview

The **Air Monitor** is an integrated indoor environmental sensing station engineered for continuous, real-time air quality observation. The device combines laser-scattering particulate measurement, optical non-dispersive infrared (NDIR) carbon dioxide sensing, and precision climate telemetry in a compact, standalone form factor.

An ARM Cortex-M4 32-bit RISC microcontroller (STM32F407ZGT6) runs a modular firmware pipeline that processes raw sensor signals, calculates the standard United States Environmental Protection Agency (US EPA) Air Quality Index (AQI), renders visual dashboards on a color IPS display, manages single-cell lithium-ion battery charging, and outputs structured telemetry.

```
+----------------------------------------------------------------------------------------------------+
|                                    AIR MONITOR SYSTEM ARCHITECTURE                                 |
+----------------------------------------------------------------------------------------------------+

   POWER DOMAIN                                              COMPUTE DOMAIN
   =============                                             ==============
  +------------------+
  | USB Type-C (5V)  |-----+
  +------------------+     |
                           v
  +------------------+  +--------------------+               +--------------------------------------+
  | 18650 Li-Ion     |->| Charger & PowerPath|               |        STM32F407ZGT6 MCU             |
  | 3.7V / 2500 mAh  |  | (BQ24040 / MCP73831)               |  - ARM Cortex-M4 @ 168 MHz with FPU  |
  +------------------+  +--------------------+               |  - 1024 KB (1 MB) Flash              |
                           |                                 |  - 192 KB SRAM (128KB + 64KB CCM)    |
                           | (VBAT / VBUS)                   |  - Package: LQFP-144 (20x20 mm)      |
                           v                                 +--------------------------------------+
                        +--------------------+                                  |
                        | 3.3V Buck Regulator|----------------------------------+ (3.3V VDD_MCU)
                        | (TPS62088 / SY8089)|
                        +--------------------+
                                   | (3.3V System Rail)
                                   +-------------------------+--------------------+
                                   |                         |                    |
                                   v                         v                    v
   SENSOR DOMAIN              +------------+            +------------+       +------------+
   =============              | PM Sensor  |            | CO2 Sensor |       | T/RH Sensor|
                              | Laser PM2.5|            | NDIR / PAS |       | Sensirion  |
                              | (PMS5003)  |            | (SCD41)    |       | (SHT41)    |
                              +------------+            +------------+       +------------+
                                   ^                         ^                    ^
                                   | UART                    | I2C                | I2C
                                   | (Candidate USART)       | (Candidate I2C1)   | (Shared Bus)
                                   |                         |                    |
                                   +-------------------------+--------------------+
                                                             |
   USER INTERFACE DOMAIN                                     |
   =====================                                     v
  +----------------------------+                     +--------------------------------------+
  | 31-Pin SPI TFT Display     |<--------------------| SPI Bus (MOSI, SCLK, CS, DC, RESET)  |
  | 2.1" IPS LCD (ST7789)      |                     | PWM Backlight Driver (TIM PWM)       |
  +----------------------------+                     +--------------------------------------+
  | Capacitive Touch Flex      |<--------------------| I2C Bus / GPIO Interrupt (TOUCH_INT) |
  | `DANY_TOUCH` (Top Slider)  |                     |                                      |
  +----------------------------+                     +--------------------------------------+
```

---

## 2. Key Features

- **Multi-Pollutant Environmental Sensing**:
  - **Particulate Matter (PM1.0, PM2.5, PM10)**: Real-time mass concentration via laser optical scattering (Plantower PMS5003-compatible over UART).
  - **Carbon Dioxide (CO₂)**: Ambient optical sensing (400–5000 ppm) via Sensirion SCD41 photoacoustic NDIR transducer over I²C.
  - **Climate Telemetry**: Precision ambient temperature (-10°C to +60°C, ±0.2°C) and relative humidity (0–100% RH, ±1.8%) via Sensirion SHT41.
- **Onboard Compute & Storage**:
  - STMicroelectronics **STM32F407ZGT6** running at 168 MHz with 1 MB embedded Flash and 192 KB SRAM.
  - Non-volatile storage for device calibration, configuration, and operating profiles.
  - Flash circular ring buffer storing up to 256 offline telemetry records during interface disconnects.
- **Display & User Interaction**:
  - High-resolution 2.1-inch color IPS TFT LCD (240 × 320) driven by Sitronix ST7789V over high-speed 40 MHz SPI.
  - Smooth 5 kHz PWM backlight dimming via low-side N-channel MOSFET driven by timer output.
  - Top capacitive touch slider bar (`DANY_TOUCH` / CST816S) enabling tap, swipe, and long-press navigation.
- **Power Management & Mobility**:
  - Dual power architecture: 5V USB Type-C external input and integrated single-cell 18650 Li-ion rechargeable battery (2500 mAh).
  - Synchronous step-down buck converter (TPS62088) providing high-efficiency 3.3V rail regulation.
  - Linear CC/CV charging controller with charge status telemetry and thermal regulation.
- **Connectivity & Telemetry**:
  - Telemetry streaming via structured JSON payloads.
  - Local discovery and telemetry interface support.
  - Robust offline data buffering in non-volatile memory.

---

## 3. Hardware Subsystem

The custom 4-layer printed circuit board (64.0 mm × 64.0 mm) is engineered for optimal signal integrity, thermal isolation, and mechanical integration:

| Component RefDes | Subsystem | Part Description | Manufacturer | Package / Footprint | Interface |
|:---|:---|:---|:---|:---|:---|
| **U1** | Processing | STM32F407ZGT6 (1MB Flash, 192KB SRAM) | STMicroelectronics | LQFP-144 (20x20 mm) | SPI, UART, I2C, GPIO |
| **U2** | Power | TPS62088 Synchronous Buck (3.3V, 1.5A) | Texas Instruments | SOT-23-6 / SOT-563 | Power Rail |
| **U3** | Power | MCP73831 1S Li-Ion Linear Charger | Microchip Technology | SOT-23-5 / DFN-8 | Power Rail / Status |
| **BAT1** | Power | 18650 Li-Ion Cell (3.7V, 2500mAh) | Industry Standard | 18.2 mm × 65.0 mm | 3-Wire Harness (VBAT/NTC/GND) |
| **M1** | Sensing | Optical Laser Dust Sensor (PM1/2.5/10) | Plantower (PMS5003) | 50.0 mm × 38.0 mm × 21.0 mm | Candidate UART |
| **M2** | Sensing | Photoacoustic NDIR CO₂ Sensor | Sensirion (SCD41) | LGA-8 10.1 mm × 10.1 mm | Candidate I²C |
| **M3** | Sensing | Precision Temperature & Humidity | Sensirion (SHT41) | DFN-4 1.5 mm × 1.5 mm | Candidate I²C |
| **DISP1** | UI | 2.1" Color IPS TFT Display (240×320) | Sitronix (ST7789V) | 31-Pin 0.5mm FPC Tail | Candidate SPI |
| **TOUCH1**| UI | Top Capacitive Touch Flex (`DANY_TOUCH`) | Custom / Hynitron | 6-Pin 0.5mm FPC Ribbon | Candidate I²C / GPIO |
| **J1** | Connector | 31-Pin 0.5mm ZIF FPC Receptacle | Hirose (FH12-31S) | SMT Right-Angle | Display Interface |
| **J2** | Connector | 6-Pin 0.5mm ZIF FPC Receptacle | Hirose (FH12-6S) | SMT Right-Angle | Touch Interface |
| **J3** | Connector | 3-Pin 2.0mm Wafer Header | JST (B3B-PH-K-S) | Through-Hole | Battery Wire Harness |
| **J4** | Connector | USB Type-C 16-Pin Receptacle | Industry Standard | Hybrid SMT / TH | 5V DC Power & Serial |

Hardware engineering files and schematics are located in [`02_Hardware/`](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/02_Hardware/).

---

## 4. Firmware Architecture

The firmware is developed in Embedded C targeting the STM32F407ZGT6 microcontroller utilizing the **STM32 HAL** and STM32Cube framework. It is architected into modular components:

```
04_Firmware/
├── CMakeLists.txt                # CMake build configuration for ARM GCC
├── main/
│   ├── main.c                   # System entrypoint & task dispatch
│   ├── system_init.c            # Power-on self-test & clock bringup (168 MHz)
│   ├── system_init.h            # System initialization headers
│   └── app_config.h             # Global hardware pin allocations & thresholds
├── components/
│   ├── drivers/                 # Hardware abstraction peripheral drivers
│   │   ├── pm_sensor_pms.c      # Plantower PMS UART packet decoder & checksum
│   │   ├── co2_sensor_scd4x.c   # Sensirion SCD41 I2C driver with CRC validation
│   │   ├── trh_sensor_sht4x.c   # Sensirion SHT41 precision T/RH driver
│   │   ├── display_st7789.c     # ST7789 40MHz SPI display driver
│   │   ├── touch_cst816.c       # Capacitive touch gesture state machine
│   │   └── power_driver.c       # Battery ADC and charging status GPIO monitor
│   ├── application/             # Measurement pipeline, AQI & alerts
│   │   ├── measurement_pipeline.c # Outlier filtering & rolling average
│   │   ├── air_quality_index.c  # US EPA AQI calculation engine
│   │   └── alert_manager.c      # Multi-threshold hysteresis alarm manager
│   ├── display/                 # Graphical user interface
│   │   ├── ui_manager.c         # Screen carousel & gesture dispatcher
│   │   ├── ui_screens.c         # Dashboard, detail, and diagnostic screen views
│   │   └── ui_theme.c           # High-contrast color tokens & typography
│   ├── network/                 # Telemetry & communications
│   │   ├── wifi_manager.c       # Network state machine & auto-reconnect
│   │   ├── mqtt_manager.c       # Structured JSON telemetry publisher
│   │   ├── http_server.c        # Local REST API endpoints
│   │   └── telemetry_encoder.c  # JSON telemetry serialization
│   ├── storage/                 # Persistence & caching
│   │   ├── nvs_manager.c        # Flash non-volatile key-value store
│   │   └── telemetry_buffer.c   # Circular ring buffer for offline records
│   ├── calibration/             # Zero-point & span calibration routines
│   ├── configuration/           # Device provisioning & factory profiles
│   ├── diagnostics/             # Peripheral self-test & fault logging
│   └── power/                   # Battery state-of-charge calculation
└── tests/                       # Unit test suites (AQI, parser, ring buffer)
```

### Measurement Pipeline Engine
1. **Sensor Acquisition**: Sensors sampled periodically via dedicated task routines.
2. **Signal Validation**: CRC and checksum verification discard malformed packets.
3. **Filtering**: Exponential Moving Average (EMA) algorithm suppresses environmental noise and transient spikes.
4. **AQI Calculation**: Evaluates US EPA piecewise linear breakpoints for PM2.5, PM10, and CO₂.
5. **UI Rendering**: Renders real-time metrics on the active display screen at ~30 FPS.
6. **Telemetry & Storage**: Serializes data to JSON, transmits via communication interfaces, or buffers to flash if offline.

---

## 5. Software Tools & Simulation Layer

The `05_Software/` directory contains an engineering toolchain for host-side validation, headless CI/CD testing, and telemetry pipeline verification without requiring bench hardware:

- **Virtual Device Simulator** (`05_Software/device_simulator/virtual_device.py`):
  - Simulates the complete environmental monitor state machine.
  - Supports 5 deterministic test scenarios: `NORMAL`, `HIGH_PM`, `HIGH_CO2`, `SENSOR_FAULT`, and `NETWORK_DROP`.
- **Telemetry Receiver Daemon** (`05_Software/telemetry_tools/telemetry_receiver.py`):
  - Ingests JSON telemetry streams and records them to a local SQLite database (`telemetry_store.sqlite3`).
- **Telemetry Inspector CLI** (`05_Software/telemetry_tools/inspect_telemetry.py`):
  - Displays real-time formatted ASCII terminal tables of sensor readings and diagnostic metrics.
- **Packet Schema Analyzer** (`05_Software/inspection_tools/packet_analyzer.py`):
  - Validates telemetry packets against schema constraints and physical sensor limits.
- **Device Provisioning Utility** (`05_Software/configuration_tools/provision_device.py`):
  - Generates configuration JSON payloads for rapid device provisioning.

---

## 6. Repository Structure

```
.
├── 01_Evidence/                 # Hardware optical analysis, inspection records & manifest
├── 02_Hardware/                 # Schematics, PCB layout, BOM, power tree & pin maps
│   ├── kicad/                  # KiCad 8.0 project, schematic, and PCB layout
│   ├── 3d/                     # Mechanical 3D STL assets for PCB and components
│   └── symbols/                # Schematic symbol libraries
├── 03_Traceability/             # Requirements, hardware, firmware & test traceability matrices
├── 04_Firmware/                 # STM32 HAL / C firmware source code & drivers
├── 05_Software/                 # Host-side simulation, telemetry receiver & analysis tools
├── 06_Tools/                    # Repository audit scripts and evidence tooling
├── 07_Documentation/            # Engineering specifications, architectural guides & manuals
├── 08_Models/                   # Mechanical enclosure CAD specification and 3D STL models
├── 09_Validation/               # System validation plans and test procedures
├── config/                      # Example configuration profiles
├── FINAL_PROJECT_COMPLETION_REPORT.md # Engineering sign-off completion report
├── TOOLCHAIN.md                 # Pinned compiler and toolchain specifications
├── CHANGELOG.md                 # Project version release history
└── LICENSE                      # Project software & documentation license
```

---

## 7. Build & Setup Instructions

### 7.1 Firmware Compilation (STM32 HAL / ARM GCC)

Prerequisites: **GNU Arm Embedded Toolchain (`arm-none-eabi-gcc`)** and **CMake** (or STM32CubeIDE).

```bash
# 1. Navigate to the firmware workspace
cd 04_Firmware

# 2. Create build directory
mkdir build && cd build

# 3. Configure CMake with ARM toolchain
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-none-eabi.cmake ..

# 4. Compile firmware binary
cmake --build .

# 5. Flash target via ST-Link / OpenOCD (SWD)
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program air_monitor.elf verify reset exit"
```

### 7.2 Running Host Simulation & Telemetry Pipeline

Prerequisites: **Python ≥ 3.10**.

```bash
# 1. Install toolchain dependencies
pip install -r 06_Tools/python/requirements.txt

# 2. Run virtual device simulator in normal operating mode
python 05_Software/device_simulator/virtual_device.py --scenario NORMAL --count 10 --output telemetry_stream.jsonl

# 3. Ingest simulated telemetry stream into SQLite database
python 05_Software/telemetry_tools/telemetry_receiver.py --file telemetry_stream.jsonl

# 4. Inspect recorded metrics
python 05_Software/telemetry_tools/inspect_telemetry.py --limit 10

# 5. Run static repository audit
python 06_Tools/python/validation/repo_audit.py
```

---

## 8. Configuration & Calibration

The system supports runtime configuration via non-volatile storage:

- **Reporting Interval**: Default telemetry frequency is 60 seconds.
- **Sensor Calibration**:
  - Particulate matter linear scaling: $PM_{cal} = (PM_{raw} \times S_{pm}) + O_{pm}$
  - Carbon dioxide zero-point calibration: Baseline 400 ppm fresh air reset via 5-second long press on the top touch slider.
  - Thermal conduction offset: Ambient temperature compensated for internal board heating ($\Delta T = -0.50^\circ\text{C}$).

Full mathematical formulations are documented in [`07_Documentation/CALIBRATION.md`](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/07_Documentation/CALIBRATION.md).

---

## 9. Engineering Documentation Index

Comprehensive technical documentation is maintained in [`07_Documentation/`](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/07_Documentation/):

- [System Architecture](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/07_Documentation/ARCHITECTURE.md)
- [Hardware Subsystem Specification](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/07_Documentation/HARDWARE.md)
- [Firmware Architecture & Driver Guide](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/07_Documentation/FIRMWARE.md)
- [Networking & Telemetry Specification](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/07_Documentation/NETWORKING.md)
- [Non-Volatile Storage & Offline Caching](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/07_Documentation/STORAGE.md)
- [Calibration Mathematical Model](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/07_Documentation/CALIBRATION.md)
- [Build & Toolchain Guide](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/07_Documentation/BUILD.md)
- [Testing & Validation Framework](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/07_Documentation/TESTING.md)
- [Host Software & Simulation Tools](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/07_Documentation/SOFTWARE.md)
- [Field Deployment & Commissioning Manual](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/07_Documentation/DEPLOYMENT.md)
- [Component Selection & Uncertainty Analysis](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/07_Documentation/UNCERTAINTY.md)
- [Operating Envelopes & Constraints](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/07_Documentation/LIMITATIONS.md)
- [Engineering Methodology Guide](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/07_Documentation/ENGINEERING_METHODOLOGY.md)
- [Comprehensive Engineering Audit](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/07_Documentation/FINAL_AUDIT.md)

---

## 10. License

- **Source Code**: MIT License. See [`LICENSE`](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/LICENSE).
- **Hardware & Documentation**: Creative Commons Attribution 4.0 International (CC BY 4.0).
