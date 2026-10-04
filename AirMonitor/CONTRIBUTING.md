# Contributing to the Air Monitor Project

Thank you for contributing to the Air Monitor embedded engineering project. We welcome pull requests for bug fixes, driver additions, optimizations, and documentation improvements.

---

## 1. Development Guidelines

### 1.1 Firmware (ESP-IDF)
- Firmware code is written in ANSI C targeting **ESP-IDF v5.3.2**.
- Follow the ESP-IDF coding style (4-space indentation, lower_snake_case for functions and variables, UPPER_SNAKE_CASE for macros and constants).
- Keep hardware drivers isolated behind standard hardware abstraction interfaces in `04_Firmware/components/drivers/include/`.
- Ensure all public functions have descriptive Doxygen headers.
- Never perform blocking calls or heap allocations inside interrupt service routines (ISRs).

### 1.2 Hardware & Schematics (KiCad)
- Hardware files are maintained in **KiCad 8.0** format.
- Ensure all schematic nets carry clear, descriptive net names.
- Verify Design Rules Check (DRC) and Electrical Rules Check (ERC) pass with zero errors before submitting pull requests.
- Update `02_Hardware/BOM.csv` and `02_Hardware/BOM.md` whenever components or footprints are modified.

### 1.3 Software Tools (Python)
- Host tools target **Python ≥ 3.10**.
- Code should adhere to PEP 8 standards (enforced via `ruff`).
- Type annotations are required for public functions and classes.

---

## 2. Testing & Quality Standards

Before submitting changes, run the automated static repository audit:
```bash
python 06_Tools/python/validation/repo_audit.py
```
Ensure that:
1. All directory structure and required file checks pass.
2. No credentials, tokens, or private keys are introduced.
3. Unit test files in `04_Firmware/tests/` compile and pass.

---

## 3. Commit Convention

We follow the Conventional Commits specification:
- `feat:` A new feature (e.g. `feat(driver): add Sensirion SPS30 support`)
- `fix:` A bug fix (e.g. `fix(ui): correct battery icon alignment`)
- `docs:` Documentation improvements (e.g. `docs(hardware): update power tree table`)
- `refactor:` Code restructuring without behavioral changes
- `test:` Adding or updating unit tests
