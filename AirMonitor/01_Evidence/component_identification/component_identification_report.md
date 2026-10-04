# Component Identification & Optical Verification Report

## 1. Scope
This document details the forensic optical identification of all integrated circuits, modules, connectors, and discrete elements observed on the Air Monitor hardware. Every identification is assigned an explicit confidence score and evidence status.

---

## 2. Microcontroller & Wireless Subsystem

### U1: ESP32-WROVER-B Module
- **Evidence Reference**: E001 (ROI C01), E004
- **Visible Markings**:
  ```text
  ESP32-WROVER-B
  CMIIT ID: 2018DP3125
  FCC ID: 2AC7Z-ESP32WROVERB
  ```
- **Observed Characteristics**:
  - Castellated 38-pin surface-mount module footprint.
  - Integrated PCB inverted-F antenna (MIFA) protruding over board edge.
  - RF shielding can with manufacturer laser engraving and 2D barcode label.
- **Documented Specification** (Espressif Systems):
  - Core: Xtensa® Dual-Core 32-bit LX6 microprocessor up to 240 MHz.
  - Flash Memory: 4 MB SPI flash integrated inside module.
  - PSRAM: 8 MB SPI pseudo-static RAM integrated inside module.
  - Connectivity: 802.11 b/g/n Wi-Fi (up to 150 Mbps) + Bluetooth v4.2 BR/EDR and BLE.
- **Hardware Constraints**:
  - Flash Bus: GPIO 6, 7, 8, 9, 10, 11 are reserved internally for flash. MUST NOT BE ACCESSED.
  - PSRAM Bus: GPIO 16 and 17 are reserved internally for PSRAM clock/chip select. MUST NOT BE ACCESSED.
  - Strapping Pins: GPIO 0, 2, 5, 12, 15 have boot-strap pull-up/down requirements.
  - Input-Only Pins: GPIO 34, 35, 36 (SENSOR_VP), 39 (SENSOR_VN) lack internal pull-up/down and output driver capability.
- **Status**: IDENTIFIED (CONFIDENCE: VERY HIGH).

---

## 3. Power Management Subsystem

### U2: Synchronous Step-Down Buck Converter (3.3V System Rail)
- **Evidence Reference**: E001 (ROI C04)
- **Observed Package**: SOT-23-6 surface-mount.
- **Surrounding Circuitry**:
  - High-saturation shielded power inductor (marked `2R2` / 2.2 µH).
  - Input MLCC filter (10 µF, 0805) and output decoupling cluster (22 µF + 100 nF).
  - Feedback divider resistors in close proximity to feedback pin.
- **Candidate ICs**:
  - TI TPS62088 / TPS62203, Silergy SY8089, Monolithic Power Systems MP2122.
- **Operating Parameters**:
  - Input: 3.0V – 5.5V (Battery / USB VBUS).
  - Output: 3.30V ± 1.5%, up to 1.5A peak current to satisfy ESP32 Wi-Fi RF transmission bursts (peaks of ~450 mA).
- **Status**: INFERRED / CANDIDATE (CONFIDENCE: HIGH).

### U3: Li-Ion Battery Charge Controller
- **Evidence Reference**: E001 (ROI C05)
- **Observed Package**: DFN-8 / SOP-8.
- **Function**: CC/CV single-cell 4.2V lithium-ion linear charge management with thermal foldback and status indication.
- **Candidate ICs**:
  - Microchip MCP73831, TI BQ24040, TP4056.
- **Status**: INFERRED / CANDIDATE (CONFIDENCE: HIGH).

---

## 4. Environmental Sensing Subsystem

### M1: Particulate Matter (PM) Sensor Module
- **Evidence Reference**: E003 (Enclosure interior)
- **Observed Form Factor**: Blue metallic rectangular module with surface cooling ribs and top centrifugal fan exhaust.
- **Physical Dimensions**: Approx 50 mm × 38 mm × 21 mm.
- **Measurement Principle**: 90-degree laser light scattering with photodiode detector and Mie scattering algorithm.
- **Interface**: Inferred standard 3.3V UART (9600-8-N-1) with active measurement transmission.
- **Candidate Modules**:
  - Plantower PMS5003 / PMS7003 / PMSA003.
  - Sensirion SPS30.
- **Status**: CANDIDATE (CONFIDENCE: HIGH for Plantower PMS-compatible form factor; exact part number UNRESOLVED).

### M2: Carbon Dioxide (CO2) & Climate Sensor Assembly
- **Evidence Reference**: E003 (Daughterboard assembly)
- **Observed Form Factor**: Vertical 2-layer PCB with dual-row 2.54 mm pin header and HC-49 crystal.
- **Candidate Modules & Technologies**:
  - Photoacoustic NDIR: Sensirion SCD40 / SCD41 (I2C interface, address `0x62`).
  - Dual-beam optical NDIR: Senseair S8 / Winsen MH-Z19 (UART interface).
  - Temperature & Relative Humidity: Sensirion SHT40 / SHT41 or SHTC3 (I2C interface, address `0x44`).
- **Status**: CANDIDATE ARCHITECTURE (CONFIDENCE: MEDIUM).

---

## 5. User Interface & Display Subsystem

### DISP1: Front Display Glass Panel
- **Evidence Reference**: E001 (J1), E004
- **Interface**: 31-pin 0.5 mm pitch bottom-contact FPC connector.
- **Technology**: Full-color IPS LCD panel (diagonal ~2.1 inch, resolution 240 × 320 or 240 × 240) driven via 4-wire SPI + D/C + RESET + BLK.
- **Candidate Controller**: Sitronix ST7789V / ST7789H2 or Ilitek ILI9341.
- **Status**: CANDIDATE (CONFIDENCE: HIGH).

### TOUCH1: Capacitive Touch Bar
- **Evidence Reference**: E001 (J2), E003
- **Marking**: `DANY_TOUCH` silkscreen on amber polyimide flex.
- **Controller**: Ultra-low power capacitive touch sensor (e.g. CST816S, Azoteq IQS263, or native ESP32 capacitive touch sensing on `TOUCH0`–`TOUCH9`).
- **Status**: INFERRED / CANDIDATE (CONFIDENCE: HIGH).

---

## 6. Summary Component Status Register

| RefDes | Subsystem | Identified Part / Candidate | Evidence ID | Status | Confidence |
|:---|:---|:---|:---|:---|:---|
| **U1** | MCU / RF | Espressif ESP32-WROVER-B | E001, E004 | IDENTIFIED | VERY HIGH |
| **U2** | Power | Synchronous Buck 3.3V (TPS62088 / SY8089) | E001 | INFERRED / CANDIDATE | HIGH |
| **U3** | Power | Li-ion Charger (BQ24040 / MCP73831) | E001 | INFERRED / CANDIDATE | HIGH |
| **BAT1** | Power | 18650 Li-Ion Cell 3.7V ~2500mAh | E003 | OBSERVED / CANDIDATE | HIGH |
| **M1** | Sensor | Plantower PMS-compatible Laser PM2.5 | E003 | CANDIDATE | HIGH |
| **M2** | Sensor | Sensirion SCD4x / NDIR CO2 Candidate | E003 | CANDIDATE | MEDIUM |
| **M3** | Sensor | Sensirion SHT4x T/RH Sensor | E001, E003 | CANDIDATE | MEDIUM |
| **DISP1** | Display | 31-pin SPI TFT LCD (ST7789 compatible) | E001, E004 | CANDIDATE | HIGH |
| **TOUCH1**| Input | Top Cap-Touch Flex (`DANY_TOUCH`) | E001, E003 | OBSERVED / INFERRED | HIGH |
| **J1** | Connector| 31-pin 0.5mm FPC Connector | E001 | OBSERVED | VERY HIGH |
| **J2** | Connector| 6-pin 0.5mm FPC Connector | E001 | OBSERVED | HIGH |
| **J3** | Connector| Battery & Power Wire Harness | E001, E003 | OBSERVED | HIGH |
