# Non-Volatile Storage & Offline Caching Architecture

## 1. Overview
The storage subsystem manages persistent configuration parameters, Wi-Fi credentials, sensor calibration coefficients, and temporary offline telemetry records on the internal 4 MB SPI flash memory.

---

## 2. Flash Partition Layout
The custom partition table (`04_Firmware/partitions.csv`) allocates memory blocks as follows:

| Partition Name | Type | Subtype | Offset | Size | Purpose |
|:---|:---|:---|:---|:---|:---|
| `nvs` | Data | NVS | `0x009000` | 24 KB (`0x006000`) | Key-value configuration parameters & Wi-Fi credentials |
| `phy_init` | Data | PHY | `0x00F000` | 4 KB (`0x001000`) | RF physical calibration parameters |
| `factory` | App | Factory | `0x010000` | 1.92 MB (`0x1E0000`)| Primary application firmware image |
| `storage` | Data | FAT | `0x1F0000` | 1.00 MB (`0x100000`)| File storage for fonts, icons, and UI assets |
| `telemetry` | Data | NVS | `0x2F0000` | 1.00 MB (`0x100000`)| Offline telemetry circular buffer cache |

---

## 3. Configuration Store (`nvs_manager.c`)
Configuration items are stored in the `air_config` NVS namespace:
- `device_id`: String (e.g. `airmonitor-001`)
- `wifi_ssid`: String
- `wifi_pass`: String
- `mqtt_uri`: String
- `report_sec`: U32 (Default: 60)
- `bright_pct`: U32 (Default: 80)

---

## 4. Offline Telemetry Ring Buffer (`telemetry_buffer.c`)
- **Capacity**: 256 records in RAM / flash sector buffer.
- **Eviction Policy**: First-In, First-Out (FIFO). When the buffer is full, the oldest uncommitted record is dropped.
- **Reconnection Flushing**: Upon Wi-Fi re-establishment, a background thread dequeues buffered records and publishes them sequentially with their original acquisition timestamps.