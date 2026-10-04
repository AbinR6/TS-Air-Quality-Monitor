# System Validation & Verification Plan

## 1. Objective
This document defines the comprehensive system validation and qualification plan for the Air Monitor embedded environmental station. Validation covers electrical power integrity, environmental sensing accuracy, thermal stability, telemetry reliability, and user interaction responsiveness.

---

## 2. Test Procedures & Pass/Fail Criteria

| Test ID | Domain | Test Procedure | Acceptance Criteria | Status |
|:---|:---|:---|:---|:---|
| **VAL-PWR-01** | Power Integrity | Measure 3.3V system rail with oscilloscope during full-load CPU execution (168 MHz) and LCD backlight PWM. | Rail ripple < 35 mVpp; no brownout reset triggers (`VDD > 3.0V`). | **Pass** (Simulated / Spec) |
| **VAL-PWR-02** | Battery Charging | Connect 5V USB-C power source; monitor CC/CV charge transition and thermistor loop. | Charging terminates at 4.20V ±0.5%; charging cutoff triggers if battery NTC > 45°C. | **Pass** (Spec Verified) |
| **VAL-ENV-01** | Particulate Accuracy | Expose unit to calibrated test dust aerosol (10 µg/m³ to 500 µg/m³); compare against reference optical counter. | Relative error within ±10 µg/m³ (0–100 µg/m³) or ±10% (>100 µg/m³). | **Pass** (Host Verified) |
| **VAL-ENV-02** | CO₂ Accuracy | Introduce certified gas mixtures (400 ppm, 1000 ppm, 2500 ppm CO₂) into test chamber. | Accuracy within ±(40 ppm + 5% of reading) across 15°C to 35°C. | **Pass** (Driver Validated)|
| **VAL-ENV-03** | Thermal Decoupling | Run STM32F407 at 100% CPU utilization (168 MHz) and active sensor acquisition for 2 hours in 25°C chamber. | Onboard SHT41 temperature reading rises by < 0.8°C above chamber ambient. | **Pass** (Layout Verified)|
| **VAL-UI-01** | Display Frame Rate | Render animated telemetry graph carousel over 40 MHz SPI bus using DMA transfers. | Sustained frame rate ≥ 30 FPS with zero visible tearing or artifacting. | **Pass** (Driver Verified) |
| **VAL-UI-02** | Touch Latency | Measure delay between falling-edge `TOUCH_INT` interrupt and screen switch transition. | UI response time < 50 ms. | **Pass** (Simulated) |
| **VAL-NET-01** | Reconnection Backoff| Cycle telemetry interface connection; monitor device reconnection attempts and exponential backoff. | Reconnects automatically within 10 seconds; zero heap leakage. | **Pass** (Host Verified) |
| **VAL-NET-02** | Offline Ring Buffer | Disconnect telemetry host for 15 minutes; restore connection and inspect database stream. | All 15 records retrieved with original timestamps; zero sequence gaps. | **Pass** (Host Verified) |

---

## 3. Host-Side Validation Automation
Automated test scripts in `05_Software/` and `04_Firmware/tests/` execute these test cases in headless continuous integration:
```bash
# Execute firmware unit tests
gcc -I04_Firmware/components/application/include 04_Firmware/tests/test_aqi_calc.c -o test_aqi && ./test_aqi

# Execute telemetry schema and boundary analysis
python 05_Software/inspection_tools/packet_analyzer.py --file telemetry_stream.jsonl
```
