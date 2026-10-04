# Hardware System Architecture

## 1. Overview
The hardware system architecture for the Air Monitor integrates environmental particulate, optical CO₂, and climate sensors with an Espressif ESP32-WROVER-B core, a color TFT display, capacitive touch navigation, and a multi-source power management subsystem.

---

## 2. System Block Diagram

```text
+----------------------------------------------------------------------------------------------------+
|                                    AIR MONITOR SYSTEM ARCHITECTURE                                 |
+----------------------------------------------------------------------------------------------------+

   POWER DOMAIN                                              COMPUTE & WIRELESS DOMAIN
   =============                                             =========================
  +------------------+
  | USB Type-C (5V)  |-----+
  +------------------+     |
                           v
  +------------------+  +--------------------+               +--------------------------------------+
  | 18650 Li-Ion     |->| Charger & PowerPath|               |        ESP32-WROVER-B MODULE         |
  | 3.7V / 2500 mAh  |  | (BQ24040 / MCP73831)               |  - Xtensa 32-bit Dual-Core LX6 240MHz|
  +------------------+  +--------------------+               |  - 4 MB Flash (Internal GPIO 6-11)   |
                           |                                 |  - 8 MB PSRAM (Internal GPIO 16-17)  |
                           | (VBAT / VBUS)                   |  - Wi-Fi 802.11 b/g/n + BLE 4.2      |
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
                                   | (TXD2/RXD2)             | (SDA/SCL)          | (Shared Bus)
                                   |                         |                    |
                                   +-------------------------+--------------------+
                                                             |
   USER INTERFACE DOMAIN                                     |
   =====================                                     v
  +----------------------------+                     +--------------------------------------+
  | 31-Pin SPI TFT Display     |<--------------------| SPI Bus (MOSI, SCLK, CS, DC, RESET)  |
  | 2.1" IPS LCD (ST7789)      |                     | PWM Backlight Driver (LEDK)          |
  +----------------------------+                     +--------------------------------------+
  | Capacitive Touch Flex      |<--------------------| I2C Bus / GPIO Interrupt (TOUCH_INT) |
  | `DANY_TOUCH` (Top Slider)  |                     |                                      |
  +----------------------------+                     +--------------------------------------+
```

---

## 3. Physical & Environmental Partitioning

1. **Airflow Isolation Tunnel**:
   - The PM laser scattering chamber requires continuous air intake without thermal contamination from the ESP32 CPU or power converters.
   - The enclosure employs a molded baffle creating an isolated laminar flow channel from the exterior louvers through the PM sensor fan port and out the exhaust louvers.
2. **Thermal Dissipation Zone**:
   - The 3.3V buck regulator, battery charge IC, and ESP32 RF power amplifier are positioned along the lower and lateral edges of Side A, heatsinked through copper ground pours and thermal vias.
   - The temperature and relative humidity sensor (SHT4x) is placed on the edge of the board in a dedicated PCB cutout (thermal relief slot) to prevent board conducted heat from biasing ambient readings.
3. **RF Antenna Clearance**:
   - The ESP32-WROVER-B PCB antenna overhangs the bottom edge of the PCB. All copper ground planes, power traces, and chassis metal are cleared in this 15 mm × 8 mm zone to ensure omnidirectional Wi-Fi/BLE radiation efficiency.
