# Hardware Optical Inspection & Analysis Index

## 1. Overview
This index catalogs the physical hardware inspection photography, high-resolution region-of-interest (ROI) crops, and optical analysis records for the Air Monitor engineering project. All primary inspection photographs are preserved with SHA-256 cryptographic checksums.

---

## 2. Hardware Inspection Photography Register

| Inspection ID | Image Filename | Hash (SHA-256) | Resolution | Target Subject | Analytical Domain |
|:---|:---|:---|:---|:---|:---|
| **E001** | `E001_main_pcb_side_a.jpeg` | `4ba4d3393950b7194639ad1942702758145a9539345cbbcefd494bcf9db7f01c` | 1600 × 1200 | Main PCB, Component Side (Side A) | PCB Layout & IC Analysis |
| **E002** | `E002_frame_display_fpc_blurred.jpeg` | `7beecb6c507df15a81ca4da7b0d2d380e227e7f91754988f4b008d58c8fa9917` | 1200 × 1600 | Internal chassis frame with display FPC | Mechanical Routing Analysis |
| **E003** | `E003_enclosure_interior.jpeg` | `5e8e81561ae358ba313175c2e0b12fe963d339b6cf4bbda8e60aa8f869a8385a` | 1200 × 1600 | Open enclosure showing sub-assemblies | Subsystem Integration Analysis |
| **E004** | `E004_display_panel_and_pcb.jpeg` | `eb81270272b12eb559eb482eb0aaec665a3983226db2a7ec8fa632ba150dfa56` | 1600 × 1200 | Front display glass panel & FPC tail | Display Subassembly Analysis |

---

## 3. Engineering Analysis Subsystems

1. **Hardware Analysis**:
   - Comprehensive optical analysis of module markings, connectors, passive networks, and test pad arrays.
   - Cross-referenced in [`01_Evidence/image_analysis/image_analysis_report.md`](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/01_Evidence/image_analysis/image_analysis_report.md).
2. **Component Identification**:
   - Verification of silicon packages (ESP32-WROVER-B, SOT-23-6 buck, DFN-8 charger, SHT41, SCD41, PMS5003).
   - Documented in [`01_Evidence/component_identification/component_identification_report.md`](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/01_Evidence/component_identification/component_identification_report.md).
3. **PCB Analysis**:
   - Routing density, 4-layer stackup constraints, thermal relief routing, and RF keepout rules.
   - Documented in [`02_Hardware/hardware_architecture.md`](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/02_Hardware/hardware_architecture.md).
4. **Display Analysis**:
   - 2.1-inch IPS panel, 31-pin 0.5mm bottom-contact FPC interface, and ST7789V driver timing.
   - Documented in [`01_Evidence/display_analysis/display_technology_report.md`](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/01_Evidence/display_analysis/display_technology_report.md).

---

## 4. Optical Analysis Manifest
The machine-readable manifest mapping image coordinates, crops, and feature overlays is available at [`01_Evidence/image_manifest.csv`](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/01_Evidence/image_manifest.csv).
