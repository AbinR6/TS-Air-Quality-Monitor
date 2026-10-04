# Build & Toolchain Guide

## 1. Overview
This guide provides instructions for building the firmware, opening hardware design files, and executing host simulation tools.

---

## 2. Toolchain Prerequisites
- **Python**: Version ≥ 3.11 with `pip`.
- **ESP-IDF**: Version v5.3.2 (targeting `esp32`).
- **KiCad**: Version 8.0 or 7.0 for viewing and editing schematics and PCB layouts.

---

## 3. Building the Firmware (ESP-IDF)
When an active ESP-IDF environment or Docker daemon is available:
```bash
# 1. Navigate to firmware directory
cd 04_Firmware

# 2. Set target to ESP32
idf.py set-target esp32

# 3. Build firmware image
idf.py build

# 4. Flash and monitor (if physical ESP32-WROVER-B is connected)
idf.py -p COM3 flash monitor
```

---

## 4. Running Host Simulation & Telemetry Tools
On standard host workstations without embedded hardware:
```bash
# Install Python requirements
pip install -r 06_Tools/python/requirements.txt

# Run virtual device simulation
python 05_Software/device_simulator/virtual_device.py --scenario NORMAL --count 5

# Run automated repository audit
python 06_Tools/python/validation/repo_audit.py
```