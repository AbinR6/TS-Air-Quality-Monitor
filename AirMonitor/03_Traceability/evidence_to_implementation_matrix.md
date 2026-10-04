# Physical Inspection & Hardware Implementation Matrix

## 1. Overview
This matrix connects physical board inspection markers to schematic symbols, layout features, and firmware implementation modules across the repository.

---

## 2. Hardware Inspection to Implementation Mapping

| Inspection Ref | Physical Feature | Technical Interpretation | Hardware Implementation | Firmware / Driver Implementation | Verification Artifact |
|:---|:---|:---|:---|:---|:---|
| **E001-C01** | `STM32F407ZGT6` physical device marking | Main ARM Cortex-M4 MCU (168MHz, 1MB Flash, 192KB SRAM) | Schematic symbol `U1`, Footprint `Package_QFP:LQFP-144_20x20mm_P0.5mm` in `air_monitor.kicad_sch` | `main/main.c`, `main/system_init.c` | Flash & SRAM resource audit |
| **E001-C02** | 31-pin 0.5mm pitch FPC receptacle with pin 1/31 markings | Display interface connector (`J1`) | Schematic symbol `J1`, Footprint `Hirose_FH12-31S-0.5SH` | `components/drivers/src/display_st7789.c` | SPI timing & pinout test |
| **E001-C03** | 6-pin 0.5mm pitch FPC receptacle | Capacitive touch interface connector (`J2`) | Schematic symbol `J2`, Footprint `Hirose_FH12-6S-0.5SH` | `components/drivers/src/touch_cst816.c` | I2C touch interrupt test |
| **E001-C04** | SOT-23-6 IC + shielded inductor marked `2R2` + MLCCs | 3.3V Step-down synchronous buck regulator | Schematic symbol `U2`, Inductor `L1` (2.2µH), MLCCs `C1-C4` | `02_Hardware/power_tree.md` | Power rail transient analysis |
| **E001-C05** | DFN-8 IC near battery input pads | 1S Li-ion linear charger (CC/CV) | Schematic symbol `U3` (MCP73831 / BQ24040) | `components/power/src/power_manager.c` | `CHG_STAT` GPIO driver test |
| **E001-C06** | PCB Silkscreen `DANY_JPU_MB_P1_2022063?` | Main board assembly identifier & revision | PCB Silkscreen text on `F.SilkS` layer in `air_monitor.kicad_pcb` | Firmware banner in `main/system_init.c` | String consistency check |
| **E001-C07** | Test pads `TP1` through `TP34` | In-Circuit Test (ICT) & factory programming pads | Footprint array `TEST_PAD_1MM` across PCB layout | Mapped in `02_Hardware/hardware_design_notes.md` | Netlist continuity review |
| **E002-C01** | Amber FPC passing through internal frame slot | Display ribbon mechanical routing and isolation | Chassis barrier model in `08_Models/enclosure_cad_spec.md` | Display dirty-region update pipeline | Mechanical clearance check |
| **E003-C01** | Blue cylindrical cell in bottom bay | 18650 Li-ion rechargeable battery (3.7V / 2500mAh) | Schematic battery net `VBAT`, 3-pin wafer `J3` | `components/power/src/power_manager.c` (ADC curve) | Battery discharge simulation |
| **E003-C02** | Blue ribbed metal rectangular module with fan | Laser optical particulate matter sensor (PMS-style) | Schematic harness `M1` (UART TX/RX/SET/RST) | `components/drivers/src/pm_sensor_pms.c` | Host packet parser unit tests |
| **E003-C03** | Secondary vertical PCB with dual-row header & crystal | CO2 / Climate sensor daughtercard assembly | Schematic header `M2` (I2C SDA/SCL/INT) | `components/drivers/src/co2_sensor_scd4x.c` | I2C mock driver tests |
| **E003-C04** | Amber flex marked `DANY_TOUCH` along top lip | Top capacitive touch slider bar | Schematic interface `TOUCH1` connected to `J2` | `components/drivers/src/touch_cst816.c` | Gesture detection state machine |
| **E004-C01** | Front glass with active display area and FPC tail | 2.1" IPS color TFT panel (ST7789 compatible) | Schematic symbol `DISP1` mated to `J1` | `components/display/src/ui_manager.c` | Display UI mock renderer |
