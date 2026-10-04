# Hardware Traceability Matrix

## 1. Overview
This matrix traces hardware components and physical specifications through to BOM line items, schematic reference designators, PCB footprints, and physical 3D models.

---

## 2. Hardware Traceability Table

| RefDes | Requirement ID | Description | Schematic Symbol | PCB Footprint | 3D Model | Status | Confidence | Engineering Specification Notes |
|:---|:---|:---|:---|:---|:---|:---|:---|:---|
| **U1** | REQ-HW-01 | STM32F407ZGT6 MCU | `MCU_ST_STM32F4:STM32F407ZGTx` | `Package_QFP:LQFP-144_20x20mm_P0.5mm` | `08_Models/stm32f407zgt6.stl` | Confirmed | Authoritative | ARM Cortex-M4 168MHz, 1MB Flash, 192KB RAM, LQFP-144 |
| **U2** | REQ-HW-02 | 3.3V Buck Regulator | `Power_Converter:TPS62088` | `Package_TO_SOT_SMD:SOT-23-6` | `02_Hardware/3d/sot23_6.stl` | Candidate | High | SOT-23-6 1.5A synchronous buck, 2.2µH coil |
| **U3** | REQ-HW-03 | Li-Ion Linear Charger | `Battery_Management:MCP73831` | `Package_SO:DFN-8-1EP_2x2mm` | Standard DFN-8 | Candidate | High | 500mA CC/CV single-cell charging from USB |
| **BAT1**| REQ-HW-02 | 18650 Li-Ion Cell | `Device:Battery_Cell` | Wire-lead pads / J3 | Standard 18650 Cylinder | Standard | High | 3.7V nominal, 2500mAh internal storage |
| **M1** | REQ-ENV-01 | Laser PM2.5 Sensor | `AirMonitor:PMS5003_Connector`| Wire harness connector | `08_Models/pm_sensor_module.stl` | Candidate | High | Plantower PMS-compatible 9600-baud UART |
| **M2** | REQ-ENV-02 | Optical CO2 Sensor | `AirMonitor:SCD41` | `Sensor:Sensirion_LGA-8` | Modular header | Candidate | High | Sensirion SCD41 photoacoustic NDIR (I2C) |
| **M3** | REQ-ENV-03 | SHT41 T/RH Sensor | `Sensor_Humidity:SHT41` | `Package_DFN_QFN:DFN-4-1EP`| PCB cutout mount | Candidate | High | Precision climate sensor on thermal cutout |
| **DISP1**| REQ-HW-04 | 2.1" IPS Color Display | `Display:ST7789V_31Pin` | Mates with J1 | `08_Models/display_subassembly.stl`| Candidate | High | 240×320 IPS panel over 40MHz 4-wire SPI |
| **TOUCH1**| REQ-HW-05 | Top Capacitive Slider | `Connector:6Pin_Touch` | Mates with J2 | Amber flex strip | Standard | High | CST816S top capacitive touch navigation |
| **J1** | REQ-HW-04 | 31-Pin 0.5mm FPC Receptacle | `Connector:FH12-31S-0.5SH` | `Connector_FFC-FPC:Hirose_FH12-31S`| FPC receptacle | Standard | Very High | 0.5mm bottom-contact horizontal FPC |
| **J2** | REQ-HW-05 | 6-Pin 0.5mm FPC Receptacle | `Connector:FH12-6S-0.5SH` | `Connector_FFC-FPC:Hirose_FH12-6S` | FPC receptacle | Standard | High | 6-conductor touch interface connector |
| **J3** | REQ-HW-02 | Battery Wafer Header | `Connector:JST_PH_B3B-PH` | `Connector_JST:JST_PH_B3B-PH-K-S` | JST PH wafer | Standard | High | Keyed 3-pin battery wire harness (VBAT/NTC/GND) |
| **J4** | REQ-HW-03 | USB Type-C Receptacle | `Connector:USB_C_Receptacle_16P`| `Connector_USB:USB_C_Receptacle` | USB-C shell | Standard | High | Base enclosure charging and console port |
| **L1** | REQ-HW-02 | 2.2µH Shielded Inductor | `Device:L_Core_Ferrite` | `Inductor_SMD:L_Coilcraft_XFL4020`| Shielded inductor | Standard | High | 2.5A saturation current, low DCR |
| **PCB** | REQ-HW-01 | Main Circuit Board | `air_monitor.kicad_sch` | `air_monitor.kicad_pcb` | `08_Models/air_monitor_pcb.stl` | Confirmed | Very High | 64mm × 64mm 4-layer FR4, matte black |
