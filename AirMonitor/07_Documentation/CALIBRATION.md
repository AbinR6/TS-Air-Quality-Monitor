# Sensor Calibration & Compensation Mathematical Model

## 1. Overview
Environmental gas and optical particulate sensors exhibit temperature drift, baseline zero-shift, and manufacturing gain variance. The calibration subsystem (`calibration_manager.c`) implements mathematical correction algorithms prior to AQI classification.

---

## 2. Calibration Mathematical Formulations

### 2.1 Particulate Matter (PM2.5) Linear Scaling
$$PM_{cal} = (PM_{raw} \times S_{pm}) + O_{pm}$$
Where:
- $S_{pm}$ = Span slope coefficient (default 1.00).
- $O_{pm}$ = Zero-point offset in µg/m³ (default 0.00).
- Bound constraint: $PM_{cal} \ge 0.0$.

### 2.2 Carbon Dioxide (CO₂) Optical Scaling
$$CO2_{cal} = (CO2_{raw} \times S_{co2}) + O_{co2}$$
Where:
- $S_{co2}$ = Calibration slope (default 1.00).
- $O_{co2}$ = Baseline zero-point offset in ppm (default 0.00).
- Baseline constraint: $CO2_{cal} \ge 400.0$ ppm (atmospheric baseline).

### 2.3 Board Thermal Conduction Compensation
Because internal power converters and RF transmission conduct heat into the PCB, ambient temperature and humidity transducers experience systematic offset:
$$T_{ambient} = T_{measured} + \Delta T_{board}$$
$$RH_{ambient} = RH_{measured} + \Delta RH_{board}$$
Empirical compensation constants:
- $\Delta T_{board} = -0.50^\circ\text{C}$
- $\Delta RH_{board} = +1.20\%$

---

## 3. Storage & Field Recalibration
Calibration constants are persisted in NVS flash and can be fine-tuned via the local REST API (`POST /api/v1/calibration`) or long-pressing the `DANY_TOUCH` top slider for 5 seconds outdoors in fresh air (400 ppm baseline reset).