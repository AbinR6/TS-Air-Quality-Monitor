# Testing, Verification & Validation Framework

## 1. Overview
The testing framework spans firmware unit tests, host-side packet and schema validation, data pipeline simulation, and automated repository compliance auditing.

---

## 2. Test Suite Architecture

### 2.1 Firmware C Unit Tests (`04_Firmware/tests/`)
- `test_aqi_calc.c`: Validates US EPA AQI piecewise linear interpolations across all breakpoint boundaries and category thresholds.
- `test_pms_parser.c`: Validates Plantower PMS 32-byte UART packet decoding, checksum generation, bit manipulation, and noise rejection.
- `test_ring_buffer.c`: Validates circular FIFO telemetry queue semantics, head/tail wrapping, and oldest-record eviction.

### 2.2 Host Software Tests (`05_Software/`)
- `virtual_device.py`: Validates deterministic state-machine transitions across operational scenarios (NORMAL, HIGH_PM, HIGH_CO2, SENSOR_FAULT, NETWORK_DROP).
- `packet_analyzer.py`: Validates telemetry JSON packet schema, key presence, and physical range sanity checks.
- `telemetry_receiver.py`: Validates SQLite relational schema creation, data ingestion, and integrity.

### 2.3 Static Analysis & Repository Compliance (`06_Tools/python/validation/`)
- `repo_audit.py`: Checks 16 directory structures, 31 mandatory engineering files, SHA-256 evidence hashes, and scans for hardcoded secrets.

---

## 3. Test Execution Status
- **Host Tools & Python Tests**: EXECUTED & VERIFIED (PASS).
- **Firmware Compilation (STM32 HAL / GCC)**: PENDING ARM CROSS-COMPILER ON LOCAL HOST (Toolchain instructions defined in BUILD.md).
- **Target Hardware Flashing**: PENDING BENCH HARDWARE ACCESS (Target flashing procedures defined in BUILD.md).