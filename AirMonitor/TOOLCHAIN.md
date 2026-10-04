# Toolchain (pinned)

| Tool | Version / image | Where it runs | Purpose |
|---|---|---|---|
| Python | ≥ 3.11 (local: 3.14.6) | host | evidence tooling, generators, host tools, tests |
| Pillow, numpy | see `06_Tools/python/pyproject.toml` | host | crops, annotation, image metrics |
| Git | any recent | host | version control |
| ARM Toolchain / STM32Cube | `arm-none-eabi-gcc` ≥ 12.3 / STM32CubeIDE v1.15+ | host / container | firmware build, target `STM32F407ZGT6` |
| KiCad | `kicad/kicad:8.0.8` (Docker) — `kicad-cli` | container | ERC/DRC/exports |
| Docker Desktop | installed | host | runs containerized toolchains + backend stack |
| Node.js | LTS (installed) | host | optional dashboard lint |

Not installed at initial audit: local KiCad, local ARM toolchain, Tesseract, clang-format, cppcheck.
These run in containers or CI instead.
