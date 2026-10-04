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
| ESP32-WROVER-B|          | Display Panel |          | I2C Sensors   |          | Touch Flex    |
| Core & Wi-Fi  |          | ST7789 + LED  |          | SCD41 + SHT41 |          | `DANY_TOUCH`  |
| 3.3V @ 150mA  |          | 3.3V @ 45mA   |          | 3.3V @ 20mA   |          | 3.3V @ 3mA    |
| (Peak 450mA)  |          | (PWM dimming) |          | (Peak 75mA)   |          |               |
+---------------+          +---------------+          +---------------+          +---------------+
```

---

## 2. Power Rail Specifications

| Rail Name | Nominal Voltage | Voltage Range | Source | Max Continuous Current | Peak Transient Current | Target Subsystems |
|:---|:---|:---|:---|:---|:---|:---|
| **VBUS** | 5.0 V | 4.5 V – 5.5 V | USB Type-C Receptacle (`J4`) | 1000 mA | 1500 mA | Battery charger, 5V PM sensor rail. |
| **VBAT** | 3.7 V | 3.0 V – 4.2 V | 18650 Li-Ion Battery (`BAT1`) | 2000 mA | 3000 mA | Charger output, buck converter input. |
| **3V3_SYS**| 3.30 V | 3.25 V – 3.35 V| Buck Converter (`U2`) | 1000 mA | 1500 mA | ESP32-WROVER-B, display logic, sensors, touch. |
| **5V_SENS**| 5.0 V | 4.75 V – 5.25 V| VBUS / Synchronous Boost | 200 mA | 350 mA | Particulate matter sensor laser diode and fan. |
| **LEDA** | 3.3 V | 3.0 V – 3.3 V | 3V3_SYS / Direct Boost | 60 mA | 80 mA | LCD backlight anode array. |

---

## 3. Power Operational States & Current Consumption

| Operating Mode | Subsystem States | Average Current (3.3V) | Battery Life (2500mAh) |
|:---|:---|:---|:---|
| **Active Measurement (Display ON, Wi-Fi Transmitting)** | ESP32 Active (240MHz), Wi-Fi TX (QoS 1), Display 100%, Fan Running, SCD41 Sampling | 280 mA | ~8.9 Hours |
| **Active Measurement (Display Dim, Wi-Fi Connected)** | ESP32 Active (160MHz), Wi-Fi DTIM3, Display 30%, PM Fan Duty 20s/60s, Sensors Active | 110 mA | ~22.7 Hours |
| **Normal Periodic Logging (Display Idle/Off)** | ESP32 Modem-Sleep, Display Standby, Sensors periodic 60s duty cycle | 35 mA | ~71.4 Hours (~3 Days) |
| **Low-Power Deep Sleep (Screen Off, Wi-Fi Off)** | ESP32 Deep-Sleep (RTC timer active), All sensors powered down | 1.8 mA | ~1380 Hours (~57 Days) |

---

## 4. Decoupling & Bulk Capacitance Strategy
- **ESP32 Power Pins**: 10 µF ceramic (X5R/0805) bulk capacitor at module input pin, backed by 100 nF (X7R/0402) on every VDD pin to ground.
- **Buck Output**: 22 µF low-ESR ceramic capacitor in parallel with 1 µF and 100 nF high-frequency bypass.
- **PM Sensor Rail**: 47 µF electrolytic/tantalum capacitor at sensor harness connector to absorb motor starting inrush current spikes.
