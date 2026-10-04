#!/usr/bin/env python3
"""
Telemetry Packet Schema Analyzer & Validator.

Validates telemetry packets against the project's data model schema and
sanity-checks bounds (e.g. PM2.5 in [0, 1000], CO2 in [350, 10000], RH in [0, 100]).
"""

import sys
import json
import argparse
from typing import Dict, Any, List

REQUIRED_KEYS = ["device_id", "seq", "timestamp", "sensors", "validity", "diagnostics"]
SENSOR_KEYS = ["pm1_0", "pm2_5", "pm10", "co2_ppm", "temperature_c", "humidity_rh", "aqi"]

def validate_packet(packet: Dict[str, Any]) -> List[str]:
    errors = []
    for key in REQUIRED_KEYS:
        if key not in packet:
            errors.append(f"Missing top-level key: '{key}'")

    if "sensors" in packet:
        s = packet["sensors"]
        for sk in SENSOR_KEYS:
            if sk not in s:
                errors.append(f"Missing sensor metric: '{sk}'")
            elif not isinstance(s[sk], (int, float)):
                errors.append(f"Metric '{sk}' is not numeric: {s[sk]}")

        # Physical sanity bounds
        if s.get("pm2_5", 0) < 0 or s.get("pm2_5", 0) > 1000:
            errors.append(f"PM2.5 out of physical bounds: {s.get('pm2_5')}")
        if s.get("co2_ppm", 0) < 300 or s.get("co2_ppm", 0) > 10000:
            errors.append(f"CO2 out of physical bounds: {s.get('co2_ppm')}")
        if s.get("humidity_rh", 0) < 0 or s.get("humidity_rh", 0) > 100:
            errors.append(f"Humidity out of physical bounds: {s.get('humidity_rh')}")

    return errors

def main():
    parser = argparse.ArgumentParser(description="Air Monitor Packet Analyzer")
    parser.add_argument("--json", default=None, help="Inline JSON string to analyze")
    parser.add_argument("--file", default=None, help="JSONL file to analyze")
    args = parser.parse_args()

    packets = []
    if args.json:
        packets.append(json.loads(args.json))
    elif args.file:
        with open(args.file, "r", encoding="utf-8") as f:
            for line in f:
                if line.strip():
                    packets.append(json.loads(line))
    else:
        # Default test packet
        sample = {
            "device_id": "airmonitor-001",
            "seq": 1,
            "timestamp": 1700000000,
            "sensors": {
                "pm1_0": 9.2, "pm2_5": 15.4, "pm10": 24.1,
                "co2_ppm": 520.0, "temperature_c": 22.8, "humidity_rh": 48.5,
                "aqi": 58
            },
            "validity": {"pm": True, "co2": True, "trh": True},
            "diagnostics": {"battery_pct": 88}
        }
        packets.append(sample)

    total = len(packets)
    valid_count = 0
    for i, pkt in enumerate(packets):
        errs = validate_packet(pkt)
        if not errs:
            valid_count += 1
            print(f"[PACKET {i+1}] PASS (Device: {pkt.get('device_id')} Seq: {pkt.get('seq')})")
        else:
            print(f"[PACKET {i+1}] FAIL: {', '.join(errs)}")

    print(f"\n[ANALYZER] Analyzed {total} packets: {valid_count} PASS, {total - valid_count} FAIL")

if __name__ == "__main__":
    main()
