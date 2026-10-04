# Component Status & Hardware Specification Matrix

## 1. Classification Definitions
- **Confirmed**: Component model verified against functional requirements, package footprints, and board specifications.
- **Candidate**: A qualified commercial component meeting all electrical and mechanical constraints where multiple pin-compatible alternatives exist.
- **Standard**: Standard industry component, passive, or mechanical connector meeting reference specifications.
- **To be verified**: Secondary assembly or variant pending extended environmental stress qualification.

---

## 2. Component Status Matrix

| Component RefDes | Subsystem | Description | Specification Source | Package / Marking | Selected / Candidate Part | Status | Confidence | Engineering Rationale / Notes |
|:---|:---|:---|:---|:---|:---|:---|:---|:---|
| **U1** | Processing | Main Microcontroller (ARM Cortex-M4) | System Spec | `STM32F407ZGT6` (LQFP144) | STMicroelectronics STM32F407ZGT6 (168 MHz, 1MB Flash, 192KB RAM) | Confirmed | Authoritative | ARM Cortex-M4 with FPU, 168 MHz, LQFP144, 1MB Flash, 192KB RAM. |
| **U2** | Power | 3.3V Step-Down Synchronous Buck | Power Tree | Inductor `2R2`, SOT-23-6 | TI TPS62088 / Silergy SY8089 | Candidate | High | High-efficiency 1.5A buck regulator, 2.2µH shielded coil, MLCC filter. |
| **U3** | Power | 1S Li-Ion Linear Battery Charger | Power Tree | DFN-8 / SOT-23-5 | Microchip MCP73831 / TI BQ24040 | Candidate | High | CC/CV single-cell lithium-ion charger programmed for 500mA USB charging. |
| **BAT1**| Power | 18650 Li-Ion Rechargeable Cell | Power Tree | Cylindrical Cell | Standard 18650 3.7V 2500mAh Li-Ion | Standard | High | Internal energy storage providing >8 hours continuous cordless runtime. |
| **M1** | Sensing | Laser Dust / Particulate Sensor | Sensor Spec | Blue Ribbed Enclosure | Plantower PMS5003 / PMS7003 series | Candidate | High | Dual-channel laser scattering mass concentration measurement via UART. |
| **M2** | Sensing | Carbon Dioxide (CO2) Sensor | Sensor Spec | Miniature Module | Sensirion SCD41 (Photoacoustic NDIR) | Candidate | High | Ultra-compact optical NDIR CO2 sensor over I2C (`0x62`) with auto-baseline. |
| **M3** | Sensing | Temperature & Relative Humidity | Sensor Spec | DFN-4 Footprint | Sensirion SHT41-AD1B | Candidate | High | Precision climate sensor mounted on PCB thermal isolation cutout. |
| **DISP1**| UI | 31-Pin SPI Color TFT LCD | UI Spec | 31-Pin FPC Ribbon | 2.1" 240×320 IPS LCD (ST7789V controller) | Candidate | High | 4-wire SPI (40 MHz) with PWM backlight dimming control. |
| **TOUCH1**| UI | Top Capacitive Touch Flex | UI Spec | `DANY_TOUCH` FPC | Polyimide capacitive touch strip (CST816S) | Standard | High | Amber top capacitive slider flex strip supporting tap and swipe gestures. |
| **J1** | Interconnect | Display Interface Connector | Schematic | 31-Pin 0.5mm pitch ZIF | Hirose FH12-31S-0.5SH or equivalent | Standard | Very High | 31-pin bottom-contact horizontal FPC receptacle. |
| **J2** | Interconnect | Touch Interface Connector | Schematic | 6-Pin 0.5mm pitch ZIF | Hirose FH12-6S-0.5SH or equivalent | Standard | High | 6-position 0.5mm horizontal FPC receptacle. |
| **J3** | Interconnect | Battery / Power Wire Harness | Schematic | 3-Pin 2.0mm Header | JST-PH 3-pin 2.0mm wafer header | Standard | High | Polarized battery connector with VBAT, NTC thermistor, and ground. |
| **J4** | Interconnect | USB-C Power/Data Receptacle | Schematic | USB-C 16-Pin SMT | Standard USB Type-C 16-pin receptacle | Standard | High | Base enclosure charging and debug console connection. |
