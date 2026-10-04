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
FIRMWARE ARCHITECTURE & HARDWARE ABSTRACTION LAYER (STM32 HAL / C)
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
- **Dedicated Pin Mapping**: External sensor peripherals and buses are systematically mapped to STM32F407ZGT6 LQFP-144 alternate functions (I2C1, USART2, SPI1/2, TIM PWM), with candidate assignments designated for verification.
- **Hardware Abstraction Layer (HAL)**: Hardware drivers are encapsulated behind pure C interfaces (`pm_sensor.h`, `co2_sensor.h`, `trh_sensor.h`, `display_driver.h`), enabling peripheral interchangeability without application layer churn.

### 3.2 Preemptive Multi-Threading & Task Scheduling
- FreeRTOS tasks execute on the 168 MHz ARM Cortex-M4 core with explicit priority scheduling:
  - **Real-Time Sensing**: High priority (5) sensor acquisition task guarantees jitter-free periodic sampling.
  - **Measurement Processing & UI**: Priority (4) data processing and display updates ensure fluid user interaction without starving acquisition.
  - **Communication / Logging**: Priority (3) telemetry and storage task handles external interfaces.

### 3.3 High-Reliability Data Resilience
- **Fail-Safe Offline Storage**: In the event of communication or external interface disconnections, incoming sensor records are committed to non-volatile storage, preventing telemetry data loss.
- **Outlier Rejection**: Raw sensor readings pass through range validation and exponential moving average (EMA) filters to prevent transient electrical or optical spikes from triggering false alarms.

---

## 4. Verification & Quality Gates
- **Static Repository Inspection**: Automated scripts audit directory structure, mandatory engineering assets, and scan for accidental secrets.
- **Deterministic Host Simulation**: Python-based virtual device simulation models environmental dynamics across operational scenarios (normal indoor, particulate smoke spike, occupancy CO₂ buildup, sensor fault, network drop) to test backend systems prior to field flashing.
- **Requirements Traceability**: Every engineering deliverable traces directly back to operational requirements.
