#!/usr/bin/env python3
"""
Device Provisioning & Configuration Utility.

Generates validated device configuration profiles and factory settings files
for the Air Monitor firmware.
"""

import os
import json
import argparse

def generate_config(device_id: str, ssid: str, password: str, mqtt_uri: str, output_file: str):
    config = {
        "schema_version": 1,
        "device_id": device_id,
        "device_name": "Air Monitor",
        "location_label": "living_room",
        "timezone": "UTC0",
        "wifi": {
            "ssid": ssid,
            "password": password
        },
        "mqtt": {
            "broker_uri": mqtt_uri,
            "port": 1883,
            "client_id": device_id,
            "topic_prefix": f"devices/{device_id}"
        },
        "reporting_interval_s": 60,
        "display": {
            "brightness_pct": 80,
            "auto_dim": True
        },
        "simulation_mode": False
    }

    with open(output_file, "w", encoding="utf-8") as f:
        json.dump(config, f, indent=2)

    print(f"[PROVISION] Configuration written to {output_file} for device '{device_id}'")

def main():
    parser = argparse.ArgumentParser(description="Air Monitor Device Provisioning Tool")
    parser.add_argument("--device-id", default="airmonitor-001", help="Device serial / unique ID")
    parser.add_argument("--ssid", default="MyHomeNetwork", help="Wi-Fi SSID")
    parser.add_argument("--password", default="", help="Wi-Fi Password")
    parser.add_argument("--mqtt", default="mqtt://192.168.1.50:1883", help="MQTT Broker URI")
    parser.add_argument("--output", default="config/device_config.json", help="Output JSON path")
    args = parser.parse_args()

    generate_config(args.device_id, args.ssid, args.password, args.mqtt, args.output)

if __name__ == "__main__":
    main()
