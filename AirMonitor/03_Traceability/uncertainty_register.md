# Component Qualification & Design Evaluation Register

## 1. Overview
This register documents technical evaluations, candidate component alternatives, tolerance analyses, and second-source qualification criteria across the Air Monitor hardware and firmware subsystems.

---

## 2. Component Evaluation Register

| ID | Subsystem / Engineering Item | Current Technical Specification | Qualification Status | Confidence | System Impact | Viable Second-Source Alternatives | Qualification Criteria |
|:---|:---|:---|:---|:---|:---|:---|:---|
| **EVAL-01** | Particulate Matter (PM) Optical Sensor | Plantower PMS5003 laser scattering sensor (9600 baud UART) | **Candidate** | High | High (Driver protocol, fan voltage, airflow baffle) | Sensirion SPS30, Honeywell HPMA115S0, Plantower PMS7003 | Laser diode lifespan test, zero-count verification, step-response calibration |
| **EVAL-02** | Carbon Dioxide (CO₂) Optical Sensor | Sensirion SCD41 photoacoustic NDIR sensor on shared I2C (`0x62`) | **Candidate** | High | High (Measurement accuracy, I2C driver integration) | Senseair S8, Winsen MH-Z19C, Cubic CM1106 | 400–5000 ppm gas chamber calibration and thermal drift compensation test |
| **EVAL-03** | Ambient Temperature & Relative Humidity Sensor | Sensirion SHT41 on dedicated thermal cutout corner (`0x44`) | **Candidate** | High | Low (Standard I2C protocol and polynomial conversion) | Sensirion SHTC3, SHT31, TI HDC2080 | Environmental chamber step test (-10°C to +50°C, 20% to 90% RH) |
| **EVAL-04** | Display Controller & Panel Glass | Sitronix ST7789V 240×320 IPS LCD via 40 MHz 4-wire SPI | **Candidate** | High | Medium (SPI timing, framebuffer double-buffering) | Ilitek ILI9341, ST7735S | Framebuffer transfer rate >30 FPS, viewing angle consistency, backlight dimming |
| **EVAL-05** | Top Capacitive Touch Navigation Strip | Hynitron CST816S on 6-pin FPC ribbon with falling-edge interrupt | **Standard** | High | Medium (Gesture detection latency and debounce) | FocalTech FT6336, discrete capacitive touch IC | Tap, swipe left, swipe right, and long-press false positive rejection |
| **EVAL-06** | System Buck Converter (3.3V System Rail) | Texas Instruments TPS62088 synchronous step-down converter | **Candidate** | High | High (System efficiency, ripple on analog/ADC rails) | Silergy SY8089, Diodes Inc AP63200 | Voltage ripple <25 mVpp under 450mA peak Wi-Fi TX burst load |
| **EVAL-07** | Battery Charging Controller | Microchip MCP73831 single-cell 500mA linear charger | **Candidate** | High | Medium (Charge termination accuracy, thermal foldback) | Texas Instruments BQ24040, Richtek RT9524 | Float voltage 4.20V ±0.5%, thermal regulation during high ambient temps |
| **EVAL-08** | Measurement Filtering Algorithm | Exponential Moving Average (EMA) with outlier rejection | **Confirmed** | High | Medium (Response time vs. transient stability) | Boxcar moving average, Kalman filter | Step-response tracking during sudden smoke spike without false alarm ringing |
| **EVAL-09** | AQI Calculation Standard | US EPA Air Quality Index piecewise linear interpolation | **Confirmed** | High | Low (Health classification thresholds) | European CAQI, WHO Air Quality Guidelines | Algorithmic test vectors verifying breakpoint boundary conditions |
| **EVAL-10** | Telemetry Protocol & Topic Schema | Standard JSON payload published to `devices/{device_id}/telemetry` | **Confirmed** | High | Low (Open IoT platform interoperability) | Home Assistant MQTT discovery format, Protobuf | Schema validation with strict type checking and range sanity checks |
| **EVAL-11** | Non-Volatile Flash Persistence | ESP-IDF NVS key-value store + circular flash ring buffer | **Confirmed** | High | Medium (Flash wear-leveling, offline data resilience) | LittleFS, raw sector write | Power-cut tolerance test during active flash write cycle |
| **EVAL-12** | Sensor Calibration Compensation Model | Multi-point linear scaling with board thermal offset compensation | **Confirmed** | High | High (Absolute accuracy under active CPU load) | Automated Background Calibration (ABC), 2-point span | Thermal soak test under 100% CPU and Wi-Fi load |

---

## 3. Hardware Abstraction Layer (HAL) Decoupling
To accommodate second-source components without application-layer changes:
- All sensor drivers adhere to pure C abstraction interfaces (`pm_sensor.h`, `co2_sensor.h`, `trh_sensor.h`, `display_driver.h`).
- Pin assignments, I2C addresses, and baud rates are centralized in `app_config.h`.
- Component swaps require modifying driver configuration without impacting the measurement pipeline or UI state machines.
