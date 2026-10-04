# Host Software & Simulation Architecture

## 1. Overview
To support automated testing, CI pipeline validation, and local development without requiring bench hardware, the project includes an engineering software toolchain located in `05_Software/`.

---

## 2. Software Modules
1. **Virtual Device Simulator** (`05_Software/device_simulator/virtual_device.py`):
   - Generates synthetic telemetry based on physical environmental models.
   - Supports 5 deterministic test scenarios:
     - `NORMAL`: Typical indoor room fluctuation.
     - `HIGH_PM`: Simulated particulate spike (cooking / smoke event).
     - `HIGH_CO2`: Occupancy buildup over time.
     - `SENSOR_FAULT`: Injects communication timeout on dust sensor.
     - `NETWORK_DROP`: Simulates Wi-Fi disconnection and offline buffering.
2. **Telemetry Receiver Daemon** (`05_Software/telemetry_tools/telemetry_receiver.py`):
   - Ingests JSON telemetry streams and logs them to a local SQLite database (`telemetry_store.sqlite3`).
3. **Telemetry Inspector CLI** (`05_Software/telemetry_tools/inspect_telemetry.py`):
   - Displays real-time formatted ASCII tables of recorded metrics.
4. **Device Provisioning Utility** (`05_Software/configuration_tools/provision_device.py`):
   - Generates NVS configuration JSON files.
5. **Packet Schema Analyzer** (`05_Software/inspection_tools/packet_analyzer.py`):
   - Validates telemetry packets against the data contract and physical boundary constraints.

---

## 3. End-to-End Host Execution Pipeline
A reviewer can execute the entire pipeline with standard Python 3.x:
```bash
# 1. Run virtual simulator in HIGH_PM scenario
python 05_Software/device_simulator/virtual_device.py --scenario HIGH_PM --count 10 --output telemetry_stream.jsonl

# 2. Ingest stream into SQLite database
python 05_Software/telemetry_tools/telemetry_receiver.py --file telemetry_stream.jsonl

# 3. Inspect stored telemetry records
python 05_Software/telemetry_tools/inspect_telemetry.py --limit 10

# 4. Validate schema compliance
python 05_Software/inspection_tools/packet_analyzer.py --file telemetry_stream.jsonl
```