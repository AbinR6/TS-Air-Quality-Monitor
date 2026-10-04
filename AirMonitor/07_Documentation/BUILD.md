# Build & Toolchain Guide

## 1. Overview
This guide provides instructions for configuring the development environment, building the STM32F407ZGT6 firmware, opening hardware design files, and executing host simulation tools.

---

## 2. Toolchain Prerequisites
- **Compiler Toolchain**: ARM GNU Toolchain (`arm-none-eabi-gcc` v10.3+ or v12.3+).
- **IDE / Environment**: STM32CubeIDE (v1.14+) or VS Code with CMake Tools & Cortex-Debug.
- **Hardware Configurator**: STM32CubeMX (v6.10+).
- **Programmer / Debugger**: ST-LINK/V2, ST-LINK/V3, or OpenOCD with CMSIS-DAP over SWD.
- **Hardware CAD**: KiCad (Version 8.0 or 7.0) for schematic and PCB inspection.
- **Python Host Tools**: Python ≥ 3.10 with standard packages.

---

## 3. Building the Firmware (STM32 HAL / CMake)
Using the CMake build configuration:
```bash
# 1. Navigate to firmware directory
cd 04_Firmware

# 2. Configure build with ARM toolchain
cmake -B build -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-none-eabi.cmake

# 3. Build firmware static library / binary
cmake --build build

# 4. Flashing via ST-LINK (using STM32_Programmer_CLI or OpenOCD)
STM32_Programmer_CLI -c port=SWD -w build/air_monitor.bin 0x08000000 -v -rst
```

Alternatively, open the project directory in **STM32CubeIDE** to build and debug directly with ST-LINK.

---

## 4. Running Host Simulation & Telemetry Tools
On standard host workstations without embedded hardware:
```bash
# 1. Run virtual device simulation
python 05_Software/device_simulator/virtual_device.py --scenario NORMAL --count 5

# 2. Run automated repository audit
python 06_Tools/python/validation/repo_audit.py
```