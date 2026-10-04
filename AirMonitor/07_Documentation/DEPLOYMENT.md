# Field Deployment & Commissioning Manual

## 1. Overview
This manual defines standard operating procedures for field deployment, physical mounting, power connection, Wi-Fi network commissioning, and backend integration of the Air Monitor embedded unit.

---

## 2. Unboxing & Physical Placement Guidelines

### 2.1 Environmental Location Criteria
- **Height**: Position the monitor 1.0 to 1.5 meters above floor level (standard human breathing zone).
- **Clearance**: Ensure at least 50 mm clearance around the side and rear air ventilation louvers to allow unobstructed intake for the particulate sensor fan.
- **Thermal Isolation**: Avoid direct sunlight exposure, heating vents, air conditioning drafts, or placement immediately above high-power electronics (e.g. desktop PCs or AV receivers) to prevent artificial temperature bias.
- **Orientation**: Place on a flat, stable horizontal surface. The top capacitive touch strip (`DANY_TOUCH`) should face upward and remain unobstructed.

### 2.2 Power Connection
- Connect a standard 5V / 1.0A (or greater) USB Type-C power adapter to port `J4` at the base of the unit.
- If operating portably on internal battery power, verify that the battery is fully charged (green indicator or 100% state of charge on UI dashboard).

---

## 3. First-Time Wi-Fi Commissioning

1. **Power-On**: Press and hold the power button (`IO0`) for 1.5 seconds. The display will show the boot screen and initialize peripheral self-tests.
2. **AP Mode / Provisioning**:
   - If no valid Wi-Fi credentials exist in NVS flash, the device enters local provisioning mode.
   - Alternatively, use the host configuration tool to generate and flash a credentials profile:
     ```bash
     python 05_Software/configuration_tools/provision_device.py --ssid "Office_WiFi" --password "NetworkKey123" --mqtt-broker "mqtt://192.168.1.50:1883"
     ```
3. **Network Connection Verification**:
   - The device connects to the 2.4 GHz 802.11 b/g/n network.
   - Upon successful DHCP assignment, the device displays the local IP address and Wi-Fi signal indicator on the system info screen.

---

## 4. Backend IoT & Telemetry Integration

### 4.1 MQTT Platform Integration
The device automatically streams measurements to the configured MQTT broker:
- **Broker Port**: 1883 (TCP) or 8883 (TLS).
- **Default Telemetry Topic**: `devices/{device_id}/telemetry`
- **QoS Level**: 1 (At least once delivery).
- **Sample Telemetry Payload**:
```json
{
  "device_id": "airmonitor-001",
  "seq": 101,
  "timestamp": 1791049027,
  "sensors": {
    "pm1_0": 8.5,
    "pm2_5": 14.2,
    "pm10": 22.0,
    "co2_ppm": 520,
    "temperature_c": 22.5,
    "humidity_rh": 48.0,
    "aqi": 55
  },
  "diagnostics": {
    "battery_pct": 95,
    "network_state": "CONNECTED"
  }
}
```

### 4.2 Local REST API Polling
For local network integrations (e.g. Home Assistant or local prometheus scrapers):
- `GET http://<device_ip>/api/v1/metrics`: Returns instantaneous sensor values.
- `GET http://<device_ip>/api/v1/status`: Returns device uptime, Wi-Fi RSSI, and battery status.

---

## 5. Maintenance & Field Recalibration
- **Air Intake Cleaning**: Every 6 months, inspect external louvers for dust accumulation and gently clear with low-pressure compressed air.
- **CO₂ Baseline Recalibration**: If CO₂ baseline drifts over time, perform fresh-air recalibration:
  1. Take the device outdoors or place near an open window in fresh ambient air (~400 ppm CO₂).
  2. Allow readings to stabilize for 10 minutes.
  3. Long-press the top `DANY_TOUCH` slider for 5 seconds to trigger automatic 400 ppm baseline zero reset.