# Power Tree & Power Distribution Network Architecture

## 1. System Power Budget & Distribution Diagram

```text
+-----------------------------------------------------------------------------------------------+
|                                      POWER TREE ARCHITECTURE                                  |
+-----------------------------------------------------------------------------------------------+

                      +---------------------+
                      |   USB-C Input (J4)  |
                      |   5.0V VBUS (500mA) |
                      +---------------------+
                                 |
                                 +-----------------------+
                                 |                       |
                                 v                       v
                      +---------------------+  +--------------------+
                      | PMIC / Li-Ion Chg   |  | 5V Boost / Passthru|-----> [M1] PM Laser Sensor
                      | U3: BQ24040/MCP73831|  | (TPS61099 / Direct)|       (Fan & Laser: 5V @ 120mA)
                      +---------------------+  +--------------------+
                                 |
                                 v
                      +---------------------+
                      | 18650 Li-Ion Cell   |
                      | 3.7V Nom (3.0V-4.2V)|
                      +---------------------+
                                 |
                                 v (VBAT_SYS)
                      +---------------------+
                      | Synchronous Buck IC |
                      | U2: TPS62088/SY8089 |
                      | 3.3V System Rail    |
                      +---------------------+
                                 |
      +--------------------------+--------------------------+--------------------------+
      |                          |                          |                          |
      v                          v                          v                          v
+---------------+          +---------------+          +---------------+          +---------------+
| STM32F407ZGT6 |          | Display Panel |          | I2C Sensors   |          | Touch Flex    |
| Core & Periph |          | ST7789 + LED  |          | SCD41 + SHT41 |          | `DANY_TOUCH`  |
| 3.3V @ 80mA   |          | 3.3V @ 45mA   |          | 3.3V @ 20mA   |          | 3.3V @ 3mA    |
| (Peak 140mA)  |          | (PWM dimming) |          | (Peak 75mA)   |          |               |
+---------------+          +---------------+          +---------------+          +---------------+
```

---

## 2. Power Rail Specifications

| Rail Name | Nominal Voltage | Voltage Range | Source | Max Continuous Current | Peak Transient Current | Target Subsystems |
|:---|:---|:---|:---|:---|:---|:---|
| **VBUS** | 5.0 V | 4.5 V – 5.5 V | USB Type-C Receptacle (`J4`) | 1000 mA | 1500 mA | Battery charger, 5V PM sensor rail. |
| **VBAT** | 3.7 V | 3.0 V – 4.2 V | 18650 Li-Ion Battery (`BAT1`) | 2000 mA | 3000 mA | Charger output, buck converter input. |
| **3V3_SYS**| 3.30 V | 3.25 V – 3.35 V| Buck Converter (`U2`) | 1000 mA | 1500 mA | STM32F407ZGT6, display logic, sensors, touch. |
| **5V_SENS**| 5.0 V | 4.75 V – 5.25 V| VBUS / Synchronous Boost | 200 mA | 350 mA | Particulate matter sensor laser diode and fan. |
| **LEDA** | 3.3 V | 3.0 V – 3.3 V | 3V3_SYS / Direct Boost | 60 mA | 80 mA | LCD backlight anode array. |

---

## 3. Power Operational States & Current Consumption

| Operating Mode | Subsystem States | Average Current (3.3V) | Battery Life (2500mAh) |
|:---|:---|:---|:---|
| **Active Measurement (Display ON, Full Speed)** | STM32 Active (168MHz), Display 100%, Fan Running, SCD41 Sampling | 180 mA | ~13.8 Hours |
| **Active Measurement (Display Dim)** | STM32 Active (84MHz), Display 30%, PM Fan Duty 20s/60s, Sensors Active | 85 mA | ~29.4 Hours |
| **Normal Periodic Logging (Display Idle/Off)** | STM32 Sleep Mode, Display Standby, Sensors periodic 60s duty cycle | 25 mA | ~100 Hours (~4.1 Days) |
| **Low-Power Standby (Screen Off, Sensors Off)** | STM32 Stop/Standby (RTC active), All sensors powered down | 0.8 mA | >2500 Hours (>100 Days) |

---

## 4. Decoupling & Bulk Capacitance Strategy
- **STM32F407 Power Pins**: 100 nF ceramic (X7R/0402) decoupling capacitor on every VDD/VSS pin pair placed immediately adjacent to package pins, plus 4.7 µF bulk ceramic cap.
- **VDDA / VSSA Analog Domain**: 100 nF ceramic cap in parallel with 1 µF ceramic, isolated via ferrite bead filter.
- **Buck Output**: 22 µF low-ESR ceramic capacitor in parallel with 1 µF and 100 nF high-frequency bypass.
- **PM Sensor Rail**: 47 µF electrolytic/tantalum capacitor at sensor harness connector to absorb motor starting inrush current spikes.
