# Air Monitor Hardware Design & PCB Notes

## 1. PCB Mechanical Form Factor & Stackup Analysis

### 1.1 Geometry & Mounting
- **Board Dimensions**: 64.0 mm width × 64.0 mm length ± 0.2 mm, corresponding to the interior envelope of the desktop air monitor enclosure.
- **Corner Profiles**: Three standard 90-degree corners with 1.5 mm radius; one distinct chamfered/rounded corner (top-right on Side A, radius ~6.0 mm) matching the asymmetric internal structural rib of the enclosure.
- **Mounting Holes**: Three plated mounting holes (diameter 2.2 mm for M2 self-tapping screws) located along perimeter margins, positioned to avoid high-voltage/RF keepout zones.

### 1.2 Layer Stackup Specification
Based on the high component density, mixed-signal routing (40 MHz SPI display bus, 400 kHz I2C sensor bus, high-speed MCU signals), and 3.3V power distribution requirements, a 4-layer FR4 stackup is specified:
- **Layer 1 (Top / Component Side)**: High-speed signal traces (SPI, I2C, UART), discrete passives, IC breakouts, local ground islands.
- **Layer 2 (Internal Ground Plane)**: Unbroken continuous 0V copper ground reference plane providing low-impedance return paths and EMI shielding.
- **Layer 3 (Internal Power Plane)**: Split power plane allocating 3V3_SYS copper flood, VBAT distribution, and 5V sensor power tracks.
- **Layer 4 (Bottom Side)**: Secondary signal interconnections, test pad breakaways, bypass return tracks, and structural thermal copper pads.

---

## 2. Thermal Management & Environmental Isolation

### 2.1 Sensor Thermal Decoupling Slot
In precision air quality monitors, thermal dissipation from microcontrollers (STM32F407 core active dissipation ~0.2W–0.4W) and power converters conducts through PCB copper, artificially elevating temperature and depressing relative humidity readings.
- **Design Implementation**: A routed mechanical slot (1.0 mm width × 15.0 mm length) isolates the SHT41 temperature/humidity transducer pad from the central ground plane, creating a thermal barrier with high thermal resistance (>120 K/W).

### 2.2 Particulate Matter Optical Chamber Airflow
The particulate matter sensor utilizes an internal brushless centrifugal fan to draw ambient air over a 650 nm laser diode.
- The chassis interior features a molded elastomer duct mating directly with the sensor inlet port, ensuring that sampled air originates exclusively from the external louvers and does not recirculate heated chassis air.

---

## 3. High-Speed Microcontroller Routing & Signal Integrity

### 3.1 Clock & High-Speed Traces
The STM32F407ZGT6 operates at core frequencies up to 168 MHz with high-speed peripherals.
- **Clock Layout**: The external crystal oscillator (HSE) is placed immediately adjacent to the OSC_IN and OSC_OUT pins, surrounded by a local ground guard ring to suppress parasitic coupling and jitter.
- **Decoupling**: Ceramic bypass capacitors (100 nF MLCCs) are placed directly at each VDD/VSS pin pair of the LQFP-144 package to maintain supply rail integrity during fast switching transients.

---

## 4. Test Pad (TP) Matrix & In-Circuit Test (ICT) Allocation

Silkscreen markings on Side A designate 34 circular gold test pads (`TP1` through `TP34`):
- **TP1–TP4**: Programming & Debug (`SWDIO`, `SWCLK`, `NRST`, `USART_TX`) [Candidate allocation].
- **TP5–TP8**: Primary Power Rails (`VBUS`, `VBAT`, `3V3_SYS`, `GND`).
- **TP9–TP12**: Display Bus Taps (`DISP_SCLK`, `DISP_MOSI`, `DISP_CS`, `DISP_DC`).
- **TP13–TP16**: I2C Bus & Touch (`I2C_SDA`, `I2C_SCL`, `TOUCH_INT`, `TOUCH_RST`).
- **TP17–TP20**: Particulate Sensor Harness (`PM_TX`, `PM_RX`, `PM_SET`, `PM_RESET`).
- **TP21–TP34**: Auxiliary I/O, ground stitch points, and factory calibration sense lines.
