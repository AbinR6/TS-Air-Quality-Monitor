# Networking, Protocols & Telemetry Specification

## 1. Overview
The Air Monitor features dual network interfaces: Wi-Fi (802.11 b/g/n) for long-range telemetry reporting, and local HTTP REST discovery.

---

## 2. Wi-Fi Station Management
- **Standard**: 802.11 b/g/n (2.4 GHz).
- **Security**: WPA2-Personal (PSK).
- **State Machine**:
  - `DISCONNECTED` -> `CONNECTING` -> `CONNECTED` -> `RECONNECTING` (exponential backoff).
- **Power Save Mode**: Modem-sleep mode enabled between reporting intervals to reduce battery drain.

---

## 3. MQTT Telemetry Protocol
- **Transport**: TCP/IP (port 1883) or TLS (port 8883).
- **Topic Hierarchy**:
  - Telemetry Upload: `devices/{device_id}/telemetry` (QoS 1).
  - Status / LWT: `devices/{device_id}/status` (Retained, `online` / `offline`).
  - Command / Config: `devices/{device_id}/command` (Subscribed).
- **Payload Schema (JSON)**:
```json
{
  "device_id": "airmonitor-001",
  "seq": 142,
  "timestamp": 1791049027,
  "sensors": {
    "pm1_0": 8.5,
    "pm2_5": 14.2,
    "pm10": 22.0,
    "co2_ppm": 485,
    "temperature_c": 22.50,
    "humidity_rh": 48.00,
    "aqi": 55
  },
  "validity": {
    "pm": true,
    "co2": true,
    "trh": true
  },
  "diagnostics": {
    "battery_pct": 82,
    "network_state": "CONNECTED"
  }
}
```

---

## 4. Local REST HTTP API
- **Port**: 80 (TCP).
- **Endpoints**:
  - `GET /api/v1/metrics`: Returns real-time sensor measurements in JSON format.
  - `GET /api/v1/status`: Returns device identity, uptime, battery SoC, and Wi-Fi RSSI.