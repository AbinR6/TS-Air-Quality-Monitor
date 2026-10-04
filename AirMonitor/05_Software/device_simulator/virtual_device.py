#!/usr/bin/env python3
"""
Virtual Device Simulator for Air Monitor.

Simulates the STM32F407ZGT6 air monitor firmware pipeline,
generating synthetic sensor telemetry according to realistic physics and
operational scenarios (NORMAL, HIGH_PM, HIGH_CO2, SENSOR_FAULT, NETWORK_DROP).
"""

import sys
import time
import json
import math
import random
import argparse
from typing import Dict, Any

SCENARIOS = ["NORMAL", "HIGH_PM", "HIGH_CO2", "SENSOR_FAULT", "NETWORK_DROP"]

class AirMonitorSimulator:
    def __init__(self, device_id: str = "airmonitor-001", scenario: str = "NORMAL"):
        self.device_id = device_id
        self.scenario = scenario
        self.seq = 0
        self.battery_pct = 95
        self.base_time = int(time.time())

    def generate_sample(self, step: int) -> Dict[str, Any]:
        self.seq += 1
        current_time = self.base_time + (step * 5)
        
        # Base realistic environmental parameters
        temp = 22.4 + 1.5 * math.sin(step * 0.1) + random.uniform(-0.1, 0.1)
        rh = 48.0 + 3.0 * math.cos(step * 0.08) + random.uniform(-0.2, 0.2)
        pm1 = 8.0 + random.uniform(-1.0, 1.0)
        pm25 = 14.5 + random.uniform(-1.5, 1.5)
        pm10 = 22.0 + random.uniform(-2.0, 2.0)
        co2 = 480.0 + 20.0 * math.sin(step * 0.05) + random.uniform(-5.0, 5.0)

        pm_valid = True
        co2_valid = True
        trh_valid = True

        # Inject Scenario Behaviors
        if self.scenario == "HIGH_PM":
            # Simulate cooking or smoke event
            spike = 85.0 * math.exp(-((step - 10) ** 2) / 25.0) if step >= 5 else 0
            pm25 += spike
            pm10 += spike * 1.6
            pm1 += spike * 0.6
        elif self.scenario == "HIGH_CO2":
            # Simulate closed room occupancy buildup
            co2 = min(2200.0, 480.0 + step * 45.0)
        elif self.scenario == "SENSOR_FAULT":
            # Simulate optical dust sensor communication dropout
            if step >= 3:
                pm_valid = False
                pm25 = -1.0
                pm10 = -1.0
                pm1 = -1.0
        elif self.scenario == "NETWORK_DROP":
            # Simulator will flag network offline
            pass

        # Compute US EPA AQI for PM2.5
        aqi = self._calculate_aqi(pm25 if pm_valid else 0)

        # Battery slow decay
        if step % 20 == 0 and self.battery_pct > 5:
            self.battery_pct -= 1

        payload = {
            "device_id": self.device_id,
            "seq": self.seq,
            "timestamp": current_time,
            "scenario": self.scenario,
            "sensors": {
                "pm1_0": round(max(0.0, pm1), 1),
                "pm2_5": round(max(0.0, pm25), 1),
                "pm10": round(max(0.0, pm10), 1),
                "co2_ppm": round(max(0.0, co2), 0),
                "temperature_c": round(temp, 2),
                "humidity_rh": round(rh, 2),
                "aqi": aqi
            },
            "validity": {
                "pm": pm_valid,
                "co2": co2_valid,
                "trh": trh_valid
            },
            "diagnostics": {
                "battery_pct": self.battery_pct,
                "network_state": "DISCONNECTED" if (self.scenario == "NETWORK_DROP" and step >= 3) else "CONNECTED"
            }
        }
        return payload

    def _calculate_aqi(self, pm25: float) -> int:
        if pm25 <= 0.0: return 0
        if pm25 <= 12.0: return int((50.0 / 12.0) * pm25)
        if pm25 <= 35.4: return int(((100.0 - 51.0) / (35.4 - 12.1)) * (pm25 - 12.1) + 51.0)
        if pm25 <= 55.4: return int(((150.0 - 101.0) / (55.4 - 35.5)) * (pm25 - 35.5) + 101.0)
        if pm25 <= 150.4: return int(((200.0 - 151.0) / (150.4 - 55.5)) * (pm25 - 55.5) + 151.0)
        return 300

def main():
    parser = argparse.ArgumentParser(description="Air Monitor Virtual Device Simulator")
    parser.add_argument("--device-id", default="airmonitor-001", help="Device identifier string")
    parser.add_argument("--scenario", choices=SCENARIOS, default="NORMAL", help="Operational simulation scenario")
    parser.add_argument("--interval", type=float, default=1.0, help="Transmission interval in seconds")
    parser.add_argument("--count", type=int, default=5, help="Number of telemetry cycles (0 for infinite)")
    parser.add_argument("--output", default=None, help="Optional JSONL log output file")
    args = parser.parse_args()

    print(f"============================================================")
    print(f"  AIR MONITOR VIRTUAL DEVICE SIMULATOR")
    print(f"  Target Device ID : {args.device_id}")
    print(f"  Active Scenario  : {args.scenario}")
    print(f"  Cycle Interval   : {args.interval}s")
    print(f"============================================================")

    sim = AirMonitorSimulator(device_id=args.device_id, scenario=args.scenario)
    step = 0

    try:
        while True:
            step += 1
            sample = sim.generate_sample(step)
            json_str = json.dumps(sample, indent=None)

            # Output formatted telemetry
            s = sample["sensors"]
            v = sample["validity"]
            diag = sample["diagnostics"]
            print(f"[SEQ {sample['seq']:04d}] Time:{sample['timestamp']} | "
                  f"PM2.5:{s['pm2_5']:5.1f} | CO2:{s['co2_ppm']:4.0f}ppm | "
                  f"Temp:{s['temperature_c']:4.1f}C | RH:{s['humidity_rh']:4.1f}% | "
                  f"AQI:{s['aqi']:3d} | Net:{diag['network_state']} | Valid(PM:{v['pm']})")

            if args.output:
                with open(args.output, "a", encoding="utf-8") as f:
                    f.write(json_str + "\n")

            if args.count > 0 and step >= args.count:
                print(f"[SIMULATOR] Completed {step} cycles. Exiting.")
                break

            time.sleep(args.interval)
    except KeyboardInterrupt:
        print("\n[SIMULATOR] Terminated by user.")

if __name__ == "__main__":
    main()
