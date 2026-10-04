# Mechanical Enclosure & 3D Component Integration Specification

## 1. Mechanical Overview
The Air Monitor enclosure is a desktop cubic form factor (~80 mm × 80 mm × 65 mm) constructed from injection-molded ABS/Polycarbonate thermoplastic. The design features a modular internal frame architecture that isolates thermal and airflow domains.

---

## 2. Component Dimensional Register

| Assembly Element | Physical Dimensions (W × H × D) | Material / Construction | Mounting Style | Associated 3D Asset |
|:---|:---|:---|:---|:---|
| **Main PCB** | 64.0 mm × 64.0 mm × 1.6 mm | 4-layer FR4, Matte Black Solder Mask | 3× M2 self-tapping screw bosses | [`air_monitor_pcb.stl`](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/08_Models/air_monitor_pcb.stl) |
| **STM32F407ZGT6** | 20.0 mm × 20.0 mm × 1.4 mm | Molded epoxy LQFP-144, copper leadframe | Surface mount soldered to Main PCB | [`stm32f407zgt6.stl`](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/08_Models/stm32f407zgt6.stl) |
| **PM Sensor (Plantower)** | 50.0 mm × 38.0 mm × 21.0 mm | Stamped aluminum casing with cooling ribs | Snapped into molded chassis bay | [`pm_sensor_module.stl`](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/08_Models/pm_sensor_module.stl) |
| **Display Subassembly**| 48.0 mm × 48.0 mm × 2.2 mm | Chemically strengthened glass + TFT panel | Double-sided die-cut acrylic foam tape | [`display_subassembly.stl`](file:///c:/Users/abina/Downloads/STM32%20Project/AirMonitor/08_Models/display_subassembly.stl) |
| **18650 Battery Cell**| Ø 18.2 mm × 65.0 mm | Steel cylindrical can, PVC shrink wrap | Snapped into base cradle with foam pads | `18650_battery_cell.stl` (Standard) |
| **DANY_TOUCH Flex** | 55.0 mm × 8.0 mm × 0.15 mm | Amber polyimide FPC with silver print | Adhesive bonded to top inner casing lip | `dany_touch_flex.stl` (Standard) |

---

## 3. Exploded Assembly Relationships

```text
               +-------------------------------------------+
               |        Top Bevel: DANY_TOUCH Bar          |
               +-------------------------------------------+
                                     |
               +-------------------------------------------+
               |        Front Glass & Display Panel        |
               +-------------------------------------------+
                                     | (31-Pin FPC Ribbon)
               +-------------------------------------------+
               |       Internal Chassis Isolation Frame    |
               +-------------------------------------------+
                    /                             \
                   v                               v
    +-----------------------------+ +-----------------------------+
    |   Main PCB (Side A & B)     | |  Particulate Optical Chamber|
    |   STM32F407ZGT6 + PMIC      | |  Air Intake / Exhaust Duct  |
    +-----------------------------+ +-----------------------------+
                   \                               /
                    \                             /
               +-------------------------------------------+
               |       18650 Lithium-Ion Battery Cradle    |
               +-------------------------------------------+
                                     |
               +-------------------------------------------+
               |        Base Enclosure with USB-C Port     |
               +-------------------------------------------+
```
