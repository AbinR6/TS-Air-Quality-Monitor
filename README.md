# TS Air Quality Monitor

An embedded environmental monitoring system engineered for continuous, real-time indoor air quality observation, particulate matter tracking, CO₂ sensing, and wireless telemetry.

---

## 📷 Hardware & Prototype Gallery

<p align="center">
  <img src="pictures/WhatsApp%20Image%202026-10-03%20at%2022.04.59.jpeg" width="45%" alt="Air Monitor Hardware 1" />
  <img src="pictures/WhatsApp%20Image%202026-10-03%20at%2022.04.59%20(1).jpeg" width="45%" alt="Air Monitor Hardware 2" />
</p>
<p align="center">
  <img src="pictures/WhatsApp%20Image%202026-10-03%20at%2022.05.00.jpeg" width="45%" alt="Air Monitor Hardware 3" />
  <img src="pictures/WhatsApp%20Image%202026-10-03%20at%2022.05.00%20(1).jpeg" width="45%" alt="Air Monitor Hardware 4" />
</p>

---

## 📁 Repository Structure

```
.
├── AirMonitor/                 # Complete embedded firmware, software & documentation
│   ├── 01_Evidence/            # Test and validation evidence
│   ├── 02_Hardware/            # Schematic, PCB layouts, KiCad files, BOM
│   ├── 03_Traceability/        # Requirements and traceability matrix
│   ├── 04_Firmware/            # ESP-IDF / C modular FreeRTOS firmware
│   ├── 05_Software/            # Python provisioning, telemetry & inspection tools
│   ├── 06_Tools/               # Analysis scripts and test utilities
│   ├── 07_Documentation/       # Architecture, Calibration, Hardware, Firmware docs
│   ├── 08_Models/              # 3D enclosure and CAD models (.stl)
│   ├── 09_Validation/          # Environmental and system validation test plans
│   ├── config/                 # Example device and MQTT configuration
│   └── README.md               # Detailed system architecture and firmware documentation
├── pictures/                   # High-resolution hardware and assembly photos
└── .vscode/                    # VS Code workspace settings
```

For the comprehensive engineering methodology, system block diagrams, register maps, and flashing guides, refer to the [AirMonitor README](AirMonitor/README.md).
