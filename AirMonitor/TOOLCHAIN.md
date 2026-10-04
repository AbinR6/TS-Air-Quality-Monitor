# Toolchain (pinned)

| Tool | Version / image | Where it runs | Purpose |
|---|---|---|---|
| Python | ≥ 3.11 (local: 3.14.6) | host | evidence tooling, generators, host tools, tests |
| Pillow, numpy | see `07_Tools/python/pyproject.toml` | host | crops, annotation, image metrics |
| Git | any recent | host | version control (real timestamps only, spec §62/§80) |
| ESP-IDF | `espressif/idf:v5.3.2` (Docker) | container | firmware build, target `esp32` |
| KiCad | `kicad/kicad:8.0.8` (Docker) — `kicad-cli` | container | ERC/DRC/exports |
| Docker Desktop | installed | host | runs the above + backend stack |
| Node.js | LTS (installed) | host | optional dashboard lint |

Not installed at Phase 0 (recorded 2026-10-03): local KiCad, local ESP-IDF, Tesseract, clang-format, cppcheck.
These run in containers or CI instead.

Image tags are proposals. They will be verified with `docker pull` before Phase 7/10 and updated here if unavailable.
