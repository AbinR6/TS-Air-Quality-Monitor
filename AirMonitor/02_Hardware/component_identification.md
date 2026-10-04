# Hardware Component Identification & Technical Dossier

## 1. Primary Processing Module (U1)
- **Component Identifier**: ESP32-WROVER-B
- **Manufacturer**: Espressif Systems
- **Silicon Revision**: ESP32-D0WD (ECO V3)
- **Physical Package**: SMT Module, 18.0 mm × 31.4 mm × 3.3 mm, 38 castellated pins.
- **Module Markings & Compliance**:
  - Top Line: `ESP32-WROVER-B`
  - Regulatory: `CMIIT ID: 2018DP3125`, `FCC ID: 2AC7Z-ESP32WROVERB`, CE mark.
  - Tracking Data: 2D DataMatrix barcode with MAC address identifier.
- **Internal Architecture**:
  - Embedded Flash: 4 MB (32 Mbit) SPI flash operating at 80 MHz on internal bus (GPIO 6–11).
  - Embedded PSRAM: 8 MB (64 Mbit) SPI pseudo-static RAM operating at 80 MHz on internal bus (GPIO 16–17).
  - Clocking: 40 MHz integrated crystal oscillator.

---

## 2. Power Conversion & Battery Charging (U2, U3)

### 2.1 Synchronous Step-Down DC-DC Converter (U2)
- **Role**: 3.3V System Logic Rail Regulator.
- **Topology**: High-frequency synchronous step-down (buck) converter.
- **Reference Candidate**: Texas Instruments TPS62088 / Silergy SY8089.
- **Pin Configuration (SOT-23-6)**:
  - Pin 1: `VIN` (Connected to VBAT/VBUS power path, decoupled with 10µF 0805 MLCC).
  - Pin 2: `GND` (Direct ground thermal pad).
  - Pin 3: `EN` (Active-high enable connected to system power controller).
  - Pin 4: `FB` (Feedback node fed by precision resistor divider 0.6V reference).
  - Pin 5: `SW` (Switching node connected to 2.2µH shielded power inductor).
  - Pin 6: `MODE` / `NC` (Fixed PWM / Power Save Mode).
- **Electrical Performance**:
  - Input: 2.7V to 5.5V.
  - Output: 3.30V regulated, ripple < 25 mVpp.
  - Efficiency: ~93% at 200 mA load.

### 2.2 Lithium-Ion Battery Charger (U3)
- **Role**: CC/CV Battery Charger for single-cell 18650 Li-ion battery.
- **Reference Candidate**: Microchip MCP73831 / TI BQ24040.
- **Operating Parameters**:
  - Charge Current: Programmed via PROG resistor to 500 mA (USB standard compliant).
  - Float Voltage: 4.20V ± 0.5%.
  - Status Indicators: Open-drain `CHG` output routed to ESP32 GPIO for charging telemetry.

---

## 3. Sensor Transducers & Modules (M1, M2, M3)

### 3.1 Particulate Matter Sensor (M1)
- **Physical Form Factor**: Rectangular metal enclosure with ribbing and internal centrifugal fan.
- **Interface**: Asynchronous Serial (UART) 9600 baud, 8 data bits, no parity, 1 stop bit.
- **Power Requirement**: 5.0V input rail for internal fan motor and laser diode; 3.3V logic level signals.
- **Data Frame Format**: 32-byte active transmission burst containing standard PM1.0, PM2.5, PM10 mass concentrations in µg/m³, particle bin counts, and checksum.

### 3.2 Carbon Dioxide Sensor (M2)
- **Architecture**: Photoacoustic NDIR CO2 sensor or optical non-dispersive infrared sensor.
- **Reference Candidate**: Sensirion SCD41.
- **Interface**: I²C bus, fixed device address `0x62`.
- **Measurement Range**: 400 ppm to 5000 ppm (accuracy ±(40 ppm + 5% of reading)).
- **Integrated Compensation**: Internal temperature and relative humidity sensor for optical cross-talk compensation.

### 3.3 Precision Temperature & Humidity Sensor (M3)
- **Component Identifier**: Sensirion SHT41-AD1B.
- **Package**: DFN-4 (1.5 mm × 1.5 mm × 0.55 mm).
- **Interface**: I²C bus, fixed device address `0x44`.
- **Accuracy**: ±0.2°C temperature accuracy; ±1.8% RH relative humidity accuracy.
- **Mounting**: Isolated corner placement with PCB thermal relief routing.

---

## 4. User Interface & Display Subassembly (DISP1, TOUCH1)

### 4.1 Display Panel (DISP1)
- **Display Type**: 2.1-inch IPS Color TFT LCD.
- **Resolution**: 240 × 320 RGB pixels.
- **Controller**: Sitronix ST7789V.
- **Interface**: 4-Wire SPI operating at 40 MHz clock frequency (`SCLK`, `MOSI`, `CS`, `DC`).
- **Connection**: 31-pin 0.5mm pitch FPC ribbon tail to J1 receptacle.

### 4.2 Capacitive Touch Strip (TOUCH1)
- **Description**: Polyimide capacitive slider flex with silkscreen identifier `DANY_TOUCH`.
- **Controller**: Hynitron CST816S or discrete capacitive sensing array.
- **Interface**: I²C bus with active-low discrete interrupt line (`TOUCH_INT`, GPIO 13).
- **Supported Gestures**: Single tap, horizontal swipe left, horizontal swipe right, long press (>5s).
