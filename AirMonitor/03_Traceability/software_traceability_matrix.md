# Host Software & Tooling Traceability Matrix

## 1. Overview
This matrix traces host-side simulation tools, telemetry receivers, configuration scripts, and static repository audit utilities to functional verification requirements and data contracts.

---

## 2. Software Traceability Table

| Tool Name | Location | Purpose & Function | Data Contract / Protocol | Verification Target | Status |
|:---|:---|:---|:---|:---|:---|
| **Virtual Device Simulator** | `05_Software/device_simulator/virtual_device.py` | Generates synthetic telemetry and simulates firmware state machine (NORMAL, HIGH_PM, HIGH_CO2, SENSOR_FAULT, NETWORK_DROP) | MQTT JSON Telemetry & HTTP REST | End-to-end telemetry ingestion without physical hardware | COMPLETE / VERIFIED |
| **Telemetry Receiver Daemon** | `05_Software/telemetry_tools/telemetry_receiver.py` | Subscribes to MQTT broker and logs telemetry to local SQLite/JSON datastore | MQTT topic `devices/{id}/telemetry` | Validates payload reception, schema parsing, and database storage | COMPLETE / VERIFIED |
| **Telemetry Inspector** | `05_Software/telemetry_tools/inspect_telemetry.py` | CLI terminal tool formatting real-time and buffered telemetry packets | JSON Telemetry Schema v1 | Verification of packet serialization and data formatting | COMPLETE / VERIFIED |
| **Device Provisioning Utility**| `05_Software/configuration_tools/provision_device.py`| Generates NVS partition binaries and configuration JSON payloads | `config/device_config.example.json` | Validates configuration integrity and factory settings generation | COMPLETE / VERIFIED |
| **Packet Schema Analyzer** | `05_Software/inspection_tools/packet_analyzer.py` | Validates telemetry packets against JSON schema and threshold rules | `config/thresholds.example.json` | Automated packet validation and linting | COMPLETE / VERIFIED |
| **Repository Static Auditor** | `06_Tools/python/validation/repo_audit.py` | Audits entire repository for broken links, missing headers, secret leaks, and BOM consistency | Repository layout & coding standard | Automated quality and compliance verification | COMPLETE / VERIFIED |
