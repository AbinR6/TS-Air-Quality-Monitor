# Environmental & Sensor Qualification Matrix

## 1. Scope
This matrix establishes the environmental qualification parameters, operational stress limits, and sensor compensation models implemented across the Air Monitor hardware and firmware stack.

---

## 2. Sensor Qualification Matrix

| Sensor Subsystem | Operating Range | Resolution | Accuracy | Baseline Drift | Compensation Method |
|:---|:---|:---|:---|:---|:---|
| **PM1.0 Mass** | 0 – 1000 µg/m³ | 1 µg/m³ | ±10% @ 100–500 µg/m³ | < 5% per 1000 hours | Periodic laser diode calibration & baseline zero offset |
| **PM2.5 Mass** | 0 – 1000 µg/m³ | 1 µg/m³ | ±10 µg/m³ (0–100), ±10% (>100) | < 5% per 1000 hours | Humidity hygroscopic growth compensation algorithm |
| **PM10 Mass** | 0 – 1000 µg/m³ | 1 µg/m³ | ±15% @ 100–500 µg/m³ | < 5% per 1000 hours | Linear gain scaling in `calibration_manager.c` |
| **CO₂ Gas** | 400 – 5000 ppm | 1 ppm | ±(40 ppm + 5% of reading) | < 10 ppm / year | Auto Baseline Calibration (ABC) + fresh air manual reset |
| **Temperature** | -10°C to +60°C | 0.01°C | ±0.2°C (0°C to 50°C) | < 0.03°C / year | PCB thermal conduction offset ($\Delta T = -0.50^\circ\text{C}$) |
| **Relative Humidity**| 0 – 100% RH | 0.01% RH | ±1.8% RH (20% to 80% RH) | < 0.25% RH / year | Temperature cross-compensation polynomial |

---

## 3. Physical Chamber Test Scenarios

1. **Cold Soak (-5°C, 24 Hours)**:
   - Evaluates LCD crystal response time and lithium-ion internal resistance increase.
   - Firmware verifies battery low-temperature charging lockout (`NTC < 0°C`).
2. **Thermal Soak (+45°C, 85% RH, 48 Hours)**:
   - Evaluates high-temperature buck converter stability and thermal cutout effectiveness.
   - SHT41 relative humidity stability verified against condensation formation.
3. **Dust Challenge Chamber (100–800 µg/m³ Arizona Road Dust)**:
   - Verifies laminar intake flow ducting and fan exhaust clearance.
   - Checksum error rate on PMS5003 UART bus verified < 0.01%.
