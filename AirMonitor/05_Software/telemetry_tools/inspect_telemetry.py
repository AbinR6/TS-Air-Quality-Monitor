#!/usr/bin/env python3
"""
Telemetry Inspector CLI tool.

Displays formatted real-time or historical telemetry records stored in SQLite
or streamed from virtual device outputs.
"""

import sys
import sqlite3
import argparse
from typing import List

def inspect_db(db_path: str, limit: int = 20):
    try:
        conn = sqlite3.connect(db_path)
        cur = conn.cursor()
        cur.execute("""
            SELECT id, device_id, seq, timestamp, pm1_0, pm2_5, pm10, co2_ppm,
                   temperature_c, humidity_rh, aqi, battery_pct, network_state, valid_pm
            FROM telemetry ORDER BY id DESC LIMIT ?
        """, (limit,))
        rows = cur.fetchall()
        conn.close()

        if not rows:
            print(f"[INSPECTOR] Database {db_path} is empty or has no records.")
            return

        print(f"\n{'ID':<5} | {'DEVICE':<14} | {'SEQ':<5} | {'TIME':<10} | {'PM2.5':<6} | {'CO2':<6} | {'TEMP':<5} | {'RH%':<5} | {'AQI':<4} | {'BAT%':<4} | {'NET':<10}")
        print("-" * 92)
        for r in reversed(rows):
            print(f"{r[0]:<5} | {r[1]:<14} | {r[2]:<5} | {r[3]:<10} | {r[5]:<6.1f} | {r[7]:<6.0f} | {r[8]:<5.1f} | {r[9]:<5.1f} | {r[10]:<4} | {r[11]:<4} | {r[12]:<10}")
        print("-" * 92 + "\n")
    except Exception as e:
        print(f"[ERROR] Could not inspect database: {e}")

def main():
    parser = argparse.ArgumentParser(description="Air Monitor Telemetry Inspector")
    parser.add_argument("--db", default="telemetry_store.sqlite3", help="Path to SQLite database")
    parser.add_argument("--limit", type=int, default=20, help="Number of recent records to display")
    args = parser.parse_args()

    inspect_db(args.db, args.limit)

if __name__ == "__main__":
    main()
