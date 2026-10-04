# Image Analysis & Visual Evidence Extraction Report

## 1. Executive Summary
This document records the visual extraction, geometric measurements, optical character recognition (OCR), and feature analysis conducted across surviving photographs E001 through E004. Analysis was performed using custom analytical pipelines in `06_Tools/python/evidence/` with 4× bicubic enhancement on critical ROI crops.

---

## 2. Image E001: Main PCB (Side A)

### 2.1 Image Metadata & Overview
- **Source File**: `E001_main_pcb_side_a.jpeg`
- **Resolution**: 1600 × 1200 pixels
- **Subject**: Main system PCB removed from enclosure, resting component-side-up (designated Side A).
- **Physical Est. Dimensions**: ~64 mm × ~64 mm square PCB with radius on top-right corner.

### 2.2 Visual Features & Physical Landmarks
1. **Main Microcontroller**:
   - Marking: `STM32F407ZGT6` (IDENTIFIED).
   - Package: LQFP-144 high-density surface-mount package (20 mm × 20 mm).
   - Architecture: ARM Cortex-M4 32-bit core @ 168 MHz with 1 MB Flash, 192 KB RAM.
   - Placement: Central compute zone with multi-point ground plane ties and decoupling capacitors.
2. **Interconnect Interfaces**:
   - **J1 (Display FPC Connector)**: 31-position 0.5 mm pitch bottom-contact FPC connector located at center-left. Pin 1 and Pin 31 silkscreen indicators observed.
   - **J2 (Touch/Sensor FPC Connector)**: ~6-position 0.5 mm pitch FPC connector located right of center.
   - **J3 (Power/Secondary Harness)**: Solder pad array / wire-to-board landing area near top-left edge.
3. **Power Management Section (Top-Left)**:
   - SOT-23-6 packaged IC adjacent to a molded power inductor (shielded, marked `2R2` or `4R7`) and ceramic input/output capacitors (0805/0603 MLCCs). Consistent with a high-efficiency step-down synchronous buck regulator converting Battery/VBUS (3.7V–5V) to 3.3V system rail.
   - DFN-8 / SOP-8 IC near battery input traces: Consistent with a single-cell Li-ion linear battery charger (e.g. TP4056 or BQ2407x architecture).
4. **Sensor Interface & Gasketed Assembly**:
   - Two SMD components nestled within a rectangular black elastomer/foam optical gasket. Characteristic of an ambient light sensor (phototransistor/I2C sensor) and IR proximity or LED indicator.
   - Red SMD component (LED or sensor element) adjacent to `TP18`.
5. **Silkscreen Markings**:
   - Primary board identifier: `DANY_JPU_MB_P1_2022063?`
   - Date / batch code: `2604` (inferred manufacturing week 26 of 2020 or internal version).
6. **Test Pad Inventory**:
   - 34 circular gold-plated test pads labeled `TP1` through `TP34`.
   - Pads clustered around MCU programming lines (SWDIO, SWCLK, NRST, UART TX/RX), power rails (VBUS, VBAT, 3V3, GND), and display/sensor control buses.

---

## 3. Image E002: Chassis Frame & Display Routing

### 3.1 Overview & Constraints
- **Source File**: `E002_frame_display_fpc_blurred.jpeg`
- **Resolution**: 1200 × 1600 pixels
- **Quality**: Severe optical motion blur; low high-frequency spatial fidelity.
- **Subject**: Matte black injection-molded structural chassis sub-assembly.

### 3.2 Key Observations
1. **Mechanical Pathing**: An amber polyimide FPC ribbon passes through a precision molded mechanical slot in the partition wall, isolating the display compartment from the rear thermal/airflow chamber.
2. **Chassis Walls**: Integral ribs and snap-fit retaining features designed to hold the display module rigid against the front housing bezel.

---

## 4. Image E003: Enclosure Interior & Sub-Assemblies

### 4.1 Overview & Architecture
- **Source File**: `E003_enclosure_interior.jpeg`
- **Resolution**: 1200 × 1600 pixels
- **Subject**: Interior view of the white ABS/PC enclosure with rear cover removed, exposing three-dimensional sub-assembly layout.

### 4.2 Sub-Assembly Analysis
1. **Energy Storage**:
   - Blue cylindrical battery cell positioned horizontally along the base.
   - Dimensions consistent with standard 18650 format (~18 mm dia × 65 mm length, nominally 2000–2600 mAh 3.7V Li-ion).
2. **Particulate Matter Sensor Sub-Assembly**:
   - Blue ribbed rectangular metal enclosure mounted in dedicated bay.
   - Geometry and air inlet/outlet ports match laser scattering PM sensors (Plantower PMS5003 / PMS7003 / Sensirion SPS30 form factor).
   - Inward-facing centrifugal fan intake aligned with exterior case louvers.
3. **Secondary Daughterboard Assembly**:
   - Vertical daughtercard mounted perpendicular to main PCB plane.
   - Features a 2×N dual-row 2.54 mm gold-plated header and an HC-49 low-profile quartz crystal.
   - Inferred: Dedicated sensor interface card, NDIR CO2 optical cell carrier, or secondary co-processor module.
4. **User Input / Capacitive Touch Flex**:
   - Amber flexible printed circuit along top interior lip with silkscreen `DANY_TOUCH`.
   - Inferred: Top-mounted capacitive touch bar providing user slide/tap navigation without mechanical buttons.
5. **Airflow Channeling**:
   - Black finned baffle/slot array forming a laminar flow tunnel between ambient vents and internal sensors, preventing internal thermal convection from distorting air quality sampling.

---

## 5. Image E004: Display Sub-Assembly & Panel

### 5.1 Overview
- **Source File**: `E004_display_panel_and_pcb.jpeg`
- **Resolution**: 1600 × 1200 pixels
- **Subject**: Front panel display unit tilted forward with FPC tail leading to main board.

### 5.2 Observations
1. **Display Glass**:
   - Flat glass front panel with black mask border and high-contrast reflective rectangular display area.
   - Form factor corresponds to ~2.0 to 2.4 inch diagonal full-color TFT LCD (typical 240×320 or 240×240 IPS panel) or custom high-density segmented/monochrome OLED/LCD.
2. **Interconnect Tail**:
   - Single 31-conductor amber FPC with black stiffener bar.
   - Matches connector J1 identified on Main PCB E001.

---

## 6. Synthesis & Spatial Relationship Diagram

```text
+--------------------------------------------------------------+
|                    ENCLOSURE TOP: DANY_TOUCH                 |
|                   (Capacitive Touch Slider)                  |
+--------------------------------------------------------------+
|                                                              |
|   +-----------------------+     +------------------------+   |
|   |                       |     |                        |   |
|   |     FRONT DISPLAY     |     |       MAIN PCB         |   |
|   |      (E004/E002)      |<===>|     (E001, Side A)     |   |
|   |  31-Pin FPC Connector |     | STM32F407ZGT6 + PMIC  |   |
|   |                       |     |                        |   |
|   +-----------------------+     +------------------------+   |
|                                             |                |
|                                     Airflow Isolation        |
|                                             |                |
|   +-----------------------+     +------------------------+   |
|   |  PM SENSOR (Optical)  |     |  DAUGHTERBOARD (E003)  |   |
|   |   Blue Ribbed Metal   |     |  CO2 / Sensor Carrier  |   |
|   |    Laser Scattering   |     |    2.54mm Pin Header   |   |
|   +-----------------------+     +------------------------+   |
|                                                              |
|   +------------------------------------------------------+   |
|   |        18650 LI-ION CELL (3.7V / 2500mAh)            |   |
|   +------------------------------------------------------+   |
|                    ENCLOSURE BASE & USB-C                    |
+--------------------------------------------------------------+
```
