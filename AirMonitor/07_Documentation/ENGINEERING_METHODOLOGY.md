# Engineering Design & Development Methodology

## 1. Engineering Philosophy
The Air Monitor project follows a rigorous, multi-disciplinary embedded engineering development lifecycle. The design flow is anchored in functional modularity, robust hardware-software co-design, strict separation of concerns, and comprehensive verification.

---

## 2. Engineering Design Flow

```text
REQUIREMENTS & USER SPECIFICATIONS
              |
              v
SYSTEM ARCHITECTURE & POWER BUDGETING
              |
              v
SCHEMATIC CAPTURE & PIN/BUS ALLOCATION (KiCad 8.0)
              |
              v
PCB LAYOUT, STACKUP & SIGNAL INTEGRITY RULES
              |
              v
FIRMWARE ARCHITECTURE & HARDWARE ABSTRACTION LAYER (ESP-IDF)
              |
              v
MEASUREMENT PIPELINE & UI CAROUSEL IMPLEMENTATION
              |
              v
HOST SIMULATION & DATA PIPELINE VERIFICATION
              |
              v
END-TO-END TRACEABILITY & REPOSITORY AUDIT
```

---

## 3. Core Development Principles

### 3.1 Hardware-Software Co-Design
- **Dedicated Pin Mapping**: External sensor peripherals are assigned strictly to uncommitted ESP32 pins, preserving the module's high-speed internal SPI flash (GPIO 6–11) and PSRAM (GPIO 16–17) buses.
- **Hardware Abstraction Layer (HAL)**: Hardware drivers are encapsulated behind pure C interfaces (`pm_sensor.h`, `co2_sensor.h`, `trh_sensor.h`, `display_driver.h`), enabling peripheral interchangeability without application layer churn.

### 3.2 Preemptive Multi-Threading & Asynchronous Telemetry
- FreeRTOS tasks are assigned explicit core affinities and priority levels:
  - **Core 1 (Real-Time Sensing)**: Sensor polling tasks run with high priority (5) to guarantee jitter-free sampling.
  - **Core 0 (Graphics & Connectivity)**: UI rendering (Priority 4) and network communications (Priority 3) execute without starving sensor acquisition.

### 3.3 High-Reliability Data Resilience
- **Fail-Safe Offline Storage**: In the event of Wi-Fi or MQTT broker disconnections, incoming sensor records are automatically committed to a circular flash ring buffer, preventing telemetry data loss.
- **Outlier Rejection**: Raw sensor readings pass through range validation and exponential moving average (EMA) filters to prevent transient electrical or optical spikes from triggering false alarms.

---

## 4. Verification & Quality Gates
- **Static Repository Inspection**: Automated scripts audit directory structure, mandatory engineering assets, and scan for accidental secrets.
- **Deterministic Host Simulation**: Python-based virtual device simulation models environmental dynamics across operational scenarios (normal indoor, particulate smoke spike, occupancy CO₂ buildup, sensor fault, network drop) to test backend systems prior to field flashing.
- **Requirements Traceability**: Every engineering deliverable traces directly back to operational requirements.
