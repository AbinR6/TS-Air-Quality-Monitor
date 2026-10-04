# System Architecture Specification

## 1. Executive Summary
The Air Monitor system is an embedded environmental monitor designed for continuous real-time measurement of particulate matter (PM1.0, PM2.5, PM10), carbon dioxide (CO₂), temperature, and relative humidity. The system architecture utilizes an STMicroelectronics STM32F407ZGT6 ARM Cortex-M4 core, a color TFT display, a capacitive touch navigation interface, and local/remote telemetry capabilities.

---

## 2. Layered Software & Hardware Architecture

```text
+-----------------------------------------------------------------------------------+
|                                 USER INTERFACE LAYER                              |
|  - Main Overview Dashboard       - PM Detail View         - CO2 Detail View       |
|  - Climate Detail View           - System Diagnostics     - Touch Gesture Handler |
+-----------------------------------------------------------------------------------+
                                          |
+-----------------------------------------------------------------------------------+
|                              APPLICATION LOGIC LAYER                              |
|  - Measurement Pipeline Engine   - EPA/WHO AQI Calculator - Alert Thresholds      |
|  - Exponential Moving Average    - Outlier Rejection      - Telemetry Formatter   |
+-----------------------------------------------------------------------------------+
                                          |
+-----------------------------------------------------------------------------------+
|                             SYSTEM SERVICES & COMMS                               |
|  - FreeRTOS Task Manager (Preemptive Scheduler)           - Flash Parameter Store |
|  - Telemetry Stream Controller   - Diagnostic Reporter    - Comm Co-Processor IF  |
|  - Offline Telemetry Ring Buffer - Power & Battery Mon    - Diagnostic Fault Log  |
+-----------------------------------------------------------------------------------+
                                          |
+-----------------------------------------------------------------------------------+
|                         HARDWARE ABSTRACTION & DRIVERS                            |
|  - PM UART Driver (PMS5003)      - CO2 I2C Driver (SCD41) - T/RH Driver (SHT41)   |
|  - Display SPI Driver (ST7789)   - Touch Driver (CST816S) - Backlight Timer PWM   |
+-----------------------------------------------------------------------------------+
                                          |
+-----------------------------------------------------------------------------------+
|                                 HARDWARE SILICON                                  |
|  - STM32F407ZGT6 (168MHz Cortex-M4, 1MB Flash, 192KB SRAM) - 18650 Li-Ion Cell    |
|  - 3.3V Synchronous Buck (TPS62088)                        - Linear Charger (MCP) |
+-----------------------------------------------------------------------------------+
```

---

## 3. FreeRTOS Task Hierarchy & Resource Budget

| Task Name | Architecture | Priority | Stack Allocation | Periodicity | Responsibilities |
|:---|:---|:---|:---|:---|:---|
| `sensor_task` | Single Core (Cortex-M4) | 3 | 512 Words (2 KB) | 1000 ms | Polls PM sensor via UART; polls CO2 and T/RH via I2C. |
| `pipe_task` | Single Core (Cortex-M4) | 3 | 512 Words (2 KB) | Event-driven| Receives raw measurements, executes EMA filter and AQI calculation. |
| `ui_task` | Single Core (Cortex-M4) | 2 | 512 Words (2 KB) | 33 ms (~30 FPS) | Renders active screen to ST7789 display and handles touch events. |
| `telemetry_task` | Single Core (Cortex-M4) | 2 | 512 Words (2 KB) | 60000 ms | Manages serial/network telemetry output and diagnostic events. |
| `pwr_task` | Single Core (Cortex-M4) | 1 | 256 Words (1 KB) | 5000 ms | Reads battery ADC voltage, computes state of charge, monitors charging. |

---

## 4. Inter-Task Communication & Synchronization
1. **Sensor to Pipeline**: Thread-safe FreeRTOS queue (`s_sensor_queue`, depth 10) passing raw sensor packets.
2. **Pipeline to Telemetry**: Thread-safe FreeRTOS queue (`s_telemetry_queue`, depth 32) passing processed and calibrated records.
3. **Touch to UI**: FreeRTOS binary semaphore / direct task notification waking `ui_task` upon falling-edge interrupt from capacitive touch controller (`TOUCH_INT`).