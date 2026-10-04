# Component Selection & Technical Qualification Register

## 1. Overview
This register documents technical trade-offs, candidate component evaluations, tolerance analyses, and second-source qualifications across the Air Monitor hardware and firmware subsystems.

---

## 2. Component Selection & Trade-Off Analysis

| Subsystem | Candidate Options | Selected Part | Evaluation Rationale & Trade-Offs | Qualification Status |
|:---|:---|:---|:---|:---|
| **Main MCU** | STM32F407ZGT6 vs. STM32F405 / STM32F429 | **STM32F407ZGT6** | 168 MHz ARM Cortex-M4 with FPU, 1 MB Flash, 192 KB SRAM in LQFP-144 package provides extensive I/O capacity, DMA controllers, and processing bandwidth for real-time sensor fusion and display rendering. | **Confirmed / Authoritative** |
| **Particulate Sensor** | Plantower PMS5003 vs. PMS7003 vs. Sensirion SPS30 | **Plantower PMS5003** | Widely available dual-channel laser scattering unit with integrated centrifugal fan, 5V power, and straightforward 9600-baud UART interface. | **Candidate** |
| **CO₂ Sensor** | Sensirion SCD41 vs. Senseair S8 vs. Winsen MH-Z19C | **Sensirion SCD41** | Ultra-compact photoacoustic NDIR package (10.1 × 10.1 mm) with low power consumption (~15–18 mA active) and integrated climate cross-compensation via I²C. | **Candidate** |
| **Climate Sensor** | Sensirion SHT41 vs. SHTC3 vs. TI HDC2080 | **Sensirion SHT41** | Exceptional precision (±0.2°C, ±1.8% RH), fast response time (<4s), ultra-low power consumption (<1.5 µA average), and high condensation resistance. | **Candidate** |
| **Display Controller** | Sitronix ST7789V vs. Ilitek ILI9341 vs. SSD1306 | **Sitronix ST7789V** | Native 240×320 RGB resolution supporting high-speed 40 MHz SPI transfers and wide-angle IPS glass panels. | **Candidate** |
| **Power Management** | Linear LDO vs. Synchronous Buck (TPS62088) | **TPS62088 Synchronous Buck** | Drops VBAT (3.7V–4.2V) to 3.3V with >92% efficiency, minimizing internal heat dissipation near sensors and extending battery life by >35% compared to linear LDOs. | **Candidate** |
| **Battery Charger** | MCP73831 vs. BQ24040 | **MCP73831** | Simple, compact SOT-23-5 linear charger with 500 mA fixed current limit, ideal for USB 2.0 port power standards. | **Candidate** |

---

## 3. Second-Source & Alternate Sourcing Strategy

1. **Particulate Matter Transducer**:
   - The modular UART abstraction (`pm_sensor.h`) supports both Plantower PMS-series and Sensirion SPS30 sensors by swapping the low-level UART parser component without modifying application logic.
2. **CO₂ Transducer**:
   - The I²C driver HAL supports pin-compatible or daughterboard alternatives (e.g. Sensirion SCD40, SCD41, or UART-based Senseair S8).
3. **Power Regulators**:
   - Footprint accommodates standard SOT-23-6 synchronous buck ICs (TPS62088, SY8089, or AP63200) with minor feedback resistor adjustments.