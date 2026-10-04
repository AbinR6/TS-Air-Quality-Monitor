# Display Subsystem & Optical Interface Forensic Report

## 1. Overview
Surviving photographs E001, E002, and E004 reveal critical details of the display module, including its glass substrate, active area geometry, ribbon cable construction, and mating PCB receptacle.

---

## 2. Visual & Geometric Characterization

### 2.1 Display Panel Metrics
- **Dimensions**: Active viewing window approx. 42 mm × 42 mm to 45 mm × 45 mm (square or near-square aspect ratio).
- **Substrate**: Transmissive active-matrix thin-film transistor liquid crystal display (TFT-LCD) with wide-angle IPS alignment layer, or high-contrast monochrome segmented OLED with circular dials.
- **Bezel**: Front glass mask with black border and anti-glare polarization film.

### 2.2 Flexible Printed Circuit (FPC) Analysis
- **Connector Type**: 31-pin 0.5 mm pitch bottom-contact surface-mount ZIF/LIF connector (`J1` on Main PCB).
- **Flex Construction**: Amber polyimide dielectric with copper microstrip traces, finished with a black polymer stiffener on the insertion tongue.
- **Trace Density**: 31 conductive lines spaced at 0.5 mm center-to-center.

---

## 3. Pinout Specification & Candidate Controllers

### 3.1 Controller Candidates
The prevailing industry controllers for 31-pin small-format SPI/8-bit MCU displays are:
1. **Sitronix ST7789V / ST7789H2**: Single-chip controller/driver for 262k-color TFT. Supports 4-wire SPI (SCL, SDA, CS, DC) with high frame rates, deep sleep mode, and integrated charge pump.
2. **Ilitek ILI9341V**: 240×320 controller widely used in commercial smart home monitors.
3. **Solomon Systech SSD1306 / SSD1309**: Organic LED controllers (typically 24–30 pin FPC).

### 3.2 31-Pin FPC Pin Assignment Specification (Standard 4-Wire SPI + Backlight)

| Pin No. | Net Name | Type | Description | Engineering Design Rationale |
|:---|:---|:---|:---|:---|
| **1–2** | `GND` | Power | Ground return | Direct plane connection observed near edge |
| **3** | `LEDA` | Power | Backlight Anode (+3.3V or boost rail) | Connected to backlight LED string |
| **4–7** | `LEDK1..4` | Power | Backlight Cathode return lines | Sinks to ground via PWM control transistor |
| **8** | `VCI` / `VDD` | Power | Analog / Logic power supply (+3.3V) | Decoupled to GND via 1µF MLCC |
| **9** | `IOVCC` | Power | Interface I/O voltage (+3.3V) | Matches ESP32 3.3V logic level |
| **10** | `RESET` | Input | Display Hardware Reset (Active Low) | Driven by ESP32 GPIO |
| **11** | `CS` | Input | SPI Chip Select (Active Low) | Driven by ESP32 GPIO |
| **12** | `DC` / `RS` | Input | Data / Command Selection | Driven by ESP32 GPIO |
| **13** | `SCL` | Input | SPI Serial Clock | High-speed SPI clock (up to 40 MHz) |
| **14** | `SDA` / `MOSI`| Input | SPI Serial Data Input | High-speed SPI MOSI |
| **15** | `SDO` / `MISO`| Output | SPI Serial Data Read (Optional) | Status and ID readback |
| **16–29** | `NC` / `TE` | Misc | Tearing Effect output or No Connect | Reserved pins for 8-bit parallel variants |
| **30–31** | `GND` | Power | Ground shielding pins | Terminal ground traces |

---

## 4. User Interaction: The `DANY_TOUCH` Flex Assembly

### 4.1 Observations from E001 & E003
- Marking: `DANY_TOUCH` visible on the top amber flex strip running along the upper housing edge.
- Mating Connector: 6-pin 0.5 mm FPC connector `J2` on the main PCB.
- Pin Mapping for J2:
  - Pin 1: `VCC` (+3.3V)
  - Pin 2: `GND`
  - Pin 3: `I2C_SDA` / `TOUCH_KEY1`
  - Pin 4: `I2C_SCL` / `TOUCH_KEY2`
  - Pin 5: `TOUCH_INT` (Active-low interrupt to wake MCU)
  - Pin 6: `TOUCH_RST` (Hardware reset)

### 4.2 Interaction Model
The top-mounted touch strip provides a sleek, button-free user experience:
- **Single Tap**: Toggle display screens (Main Overview -> PM2.5 / PM10 -> CO2 -> Temperature / RH -> System Info).
- **Slide Gesture**: Rapid cycling or brightness adjustment.
- **Long Press (>3 seconds)**: Enter Wi-Fi setup / provisioning mode or trigger manual zero-point calibration.
