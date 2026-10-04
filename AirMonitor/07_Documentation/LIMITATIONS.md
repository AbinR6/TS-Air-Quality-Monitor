# Engineering Limitations & Operational Boundaries

## 1. Environmental & Measurement Boundaries

### 1.1 Operating Temperature & Humidity Envelope
- **Operating Temperature**: 0°C to +45°C. Operating outside this range induces measurement drift in the optical CO₂ and particulate sensors.
- **Operating Relative Humidity**: 5% to 95% RH (non-condensing). High condensing humidity leads to particle hygroscopic swelling, causing overestimation of optical PM mass concentration.
- **Atmospheric Pressure**: Optimized for standard atmospheric pressures (800–1100 hPa). The SCD41 optical absorption model benefits from ambient pressure compensation when deployed at high altitudes (>1500m).

### 1.2 Particulate Sensor Sampling Constraints
- **Sampling Mode**: Laser diode and centrifugal fan lifetime is optimized by running the PM sensor in periodic duty-cycled active mode (e.g. 30 seconds active measurement every 60 seconds) rather than 24/7 continuous operation.
- **Maximum Particle Mass Concentration**: Linear dynamic range is 0 to 1000 µg/m³ for PM2.5. Beyond 1000 µg/m³ (e.g. extreme wildfire smoke), optical saturation occurs.

### 1.3 Carbon Dioxide Sensor Stabilization
- **Initial Warm-up Time**: Requires approximately 60 seconds after power-on before optical photoacoustic readings reach baseline accuracy.
- **Automatic Baseline Calibration (ABC)**: Requires periodic exposure to fresh indoor/outdoor air (~400 ppm CO₂) over a 7-day cycle to maintain zero-point accuracy.

---

## 2. Electrical & Power Envelope

### 2.1 Battery Run-Time Limits
- **Cell Capacity**: Single 18650 Lithium-Ion cell (3.7V nominal, 2500 mAh / 9.25 Wh).
- **Run-time Budget**:
  - Full Active Mode (STM32 core active 168 MHz, LCD 100% brightness, continuous PM fan): ~6.5 hours.
  - Standard Eco Mode (Periodic sensor sleep, 60% LCD brightness): ~10.5 hours.
  - Low-Power / Stop Mode: >96 hours.
- **Charging Rate**: USB-C charge controller is fixed to 500 mA (MCP73831 / candidate charger) to guarantee compatibility with all standard USB 2.0 host ports and power bricks, requiring ~5 hours for a 0–100% full recharge.

### 2.2 SPI Display Refresh Ceiling
- **Display Bus Clock**: Up to 40 MHz SPI clock.
- **Frame Rate Limit**: Full-screen 240 × 320 RGB565 buffer transfers take approximately 30.7 ms, capping theoretical maximum refresh at ~32.5 FPS. This is ideal for animated UI graphs and gauges without impacting real-time sensor acquisition.

---

## 3. Host Environment & Toolchain Constraints

1. **Firmware Cross-Compilation**:
   - The embedded firmware is written targeting the STM32F407ZGT6 architecture using STM32CubeIDE / STM32 HAL / arm-none-eabi-gcc.
   - Building the native binary requires the ARM GNU toolchain (`arm-none-eabi-gcc`), CMake / Make, or STM32CubeIDE.
2. **Bench Testing**:
   - Automated testing on standard host machines is fully supported via the Python device simulator (`05_Software/device_simulator/virtual_device.py`), unit test parsers, and schema validation tools.