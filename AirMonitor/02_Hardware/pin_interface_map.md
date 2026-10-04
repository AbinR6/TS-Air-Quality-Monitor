# ESP32-WROVER-B Pin Allocation & Hardware Interface Map

## 1. Overview
The ESP32-WROVER-B module integrates an ESP32-D0WD core, a 4 MB SPI flash memory chip, and an 8 MB SPI PSRAM chip. Due to internal wiring within the module packaging, specific pins are strictly reserved and prohibited from external assignment.

---

## 2. Silicon & Package Constraints

### 2.1 Strictly Reserved Internal Pins (DO NOT USE)
- **GPIO 6 (CLK)**: Internal SPI Flash Clock
- **GPIO 7 (SD0)**: Internal SPI Flash Data 0
- **GPIO 8 (SD1)**: Internal SPI Flash Data 1
- **GPIO 9 (SD2)**: Internal SPI Flash Data 2
- **GPIO 10 (SD3)**: Internal SPI Flash Data 3
- **GPIO 11 (CMD)**: Internal SPI Flash Command
- **GPIO 16**: Connected internally to PSRAM Chip Select / Clock
- **GPIO 17**: Connected internally to PSRAM Data

### 2.2 Boot Strapping Pins (Observe State at Reset)
- **GPIO 0**: High = Normal SPI Flash boot; Low = UART download bootloader mode. Pulled high via 10 kΩ resistor with user button bypass to GND.
- **GPIO 2**: Must be floating or Low during UART flashing. Connected to ground or unasserted at boot.
- **GPIO 5**: High at boot to select default SDIO timing. Internal weak pull-up.
- **GPIO 12**: Controls VDD_SDIO voltage (0 = 3.3V, 1 = 1.8V). Must remain Low at boot to ensure 3.3V flash rail.
- **GPIO 15**: Outputs PWM boot log if High. Pulled high.

### 2.3 Input-Only Pins (GPI)
- **GPIO 34**: Input only (No internal pull-up/pull-down capability). Used for `CHG_STAT`.
- **GPIO 35**: Input only (No internal pull-up/pull-down capability). Used for `BAT_SENSE` (ADC1_CH7).
- **GPIO 36 (SENSOR_VP)**: Input only. Uncommitted / external sensor input.
- **GPIO 39 (SENSOR_VN)**: Input only. Uncommitted / external sensor input.

---

## 3. Complete Pinout Matrix

| ESP32 Pin | Functional Name | Pad Type | Signal Assignment | Direction | Electrical Characteristics |
|:---|:---|:---|:---|:---|:---|
| **1** | `GND` | Ground | System Ground | Ground | 0V Reference plane |
| **2** | `3V3` | Power | 3V3_SYS Logic Rail | Power | 3.3V Regulated (up to 500mA peak) |
| **3** | `EN` | Input | Chip Enable / Reset | Input | 10k pull-up to 3V3 + 1µF MLCC to GND |
| **4** | `SENSOR_VP` (IO36) | GPI | Uncommitted / TP1 | Input | Analog/Digital input |
| **5** | `SENSOR_VN` (IO39) | GPI | Uncommitted / TP2 | Input | Analog/Digital input |
| **6** | `IO34` | GPI | `CHG_STAT` | Input | Active-low battery charging indicator |
| **7** | `IO35` | GPI | `BAT_SENSE` | Input (ADC1_CH7) | Resistor divider 1:2 from VBAT |
| **8** | `IO32` | GPIO | `DISP_RST` | Output | Display hardware reset (Active Low) |
| **9** | `IO33` | GPIO | `DISP_DC` | Output | Display Data / Command selector |
| **10** | `IO25` | GPIO | `DISP_CS` | Output | Display SPI Chip Select (Active Low) |
| **11** | `IO26` | GPIO | `DISP_SCLK` | Output | Display SPI Clock (40 MHz) |
| **12** | `IO27` | GPIO | `DISP_MOSI` | Output | Display SPI Master-Out Slave-In |
| **13** | `IO14` | GPIO | `DISP_BL_PWM` | Output (LEDC) | Backlight PWM control (5 kHz) |
| **14** | `IO12` | GPIO | Strapping (MTDI) | Output | Boot strapping Low (3.3V Flash) |
| **15** | `GND` | Ground | Ground Plane | Ground | 0V Reference |
| **16** | `IO13` | GPIO | `TOUCH_INT` | Input (EXT_INT)| Capacitive touch interrupt |
| **17** | `SD2` (IO9) | Internal | RESERVED FLASH | N/A | PROHIBITED (Internal Flash) |
| **18** | `SD3` (IO10)| Internal | RESERVED FLASH | N/A | PROHIBITED (Internal Flash) |
| **19** | `CMD` (IO11)| Internal | RESERVED FLASH | N/A | PROHIBITED (Internal Flash) |
| **20** | `CLK` (IO6) | Internal | RESERVED FLASH | N/A | PROHIBITED (Internal Flash) |
| **21** | `SD0` (IO7) | Internal | RESERVED FLASH | N/A | PROHIBITED (Internal Flash) |
| **22** | `SD1` (IO8) | Internal | RESERVED FLASH | N/A | PROHIBITED (Internal Flash) |
| **23** | `IO15` | GPIO | Strapping (MTDO) | Output | Pulled High at boot |
| **24** | `IO2` | GPIO | Boot Strapping | Input/Output | Pulled Low at boot |
| **25** | `IO0` | GPIO | `BOOT_KEY` / `PWR_BTN` | Input | Active-low boot select / power button |
| **26** | `IO4` | GPIO | `TOUCH_RST` | Output | Touch controller hardware reset |
| **27** | `IO16` | Internal | RESERVED PSRAM | N/A | PROHIBITED (Internal PSRAM) |
| **28** | `IO17` | Internal | RESERVED PSRAM | N/A | PROHIBITED (Internal PSRAM) |
| **29** | `IO5` | GPIO | `PM_RESET` | Output | Particulate sensor reset |
| **30** | `IO18` | GPIO | `PM_UART_TX` | Output (UART2 TX)| Serial data to PM sensor |
| **31** | `IO19` | GPIO | `PM_UART_RX` | Input (UART2 RX) | Serial data from PM sensor |
| **32** | `GND` | Ground | Ground Plane | Ground | 0V Reference |
| **33** | `IO21` | GPIO | `I2C_SDA` | Bidirectional | Shared I2C Data line (4.7k pull-up) |
| **34** | `RXD0` (IO3) | GPIO | `UART0_RXD` | Input | Flashing & Console RX |
| **35** | `TXD0` (IO1) | GPIO | `UART0_TXD` | Output | Flashing & Console TX |
| **36** | `IO22` | GPIO | `I2C_SCL` | Bidirectional | Shared I2C Clock line (4.7k pull-up) |
| **37** | `IO23` | GPIO | `PM_SET` | Output | Active-high sensor run/sleep control |
| **38** | `GND` | Ground | Ground Plane | Ground | 0V Reference |
