#!/usr/bin/env python3
"""
Telemetry Receiver Daemon for Air Monitor.

Receives incoming telemetry packets (via stdin stream, file pipe, or socket)
and logs records to a structured local SQLite database.
"""

import sys
import os
import json
import sqlite3
import argparse
from typing import Dict, Any

DEFAULT_DB_PATH = "telemetry_store.sqlite3"

def init_db(db_path: str):
    conn = sqlite3.connect(db_path)
    cur = conn.cursor()
    cur.execute("""
        CREATE TABLE IF NOT EXISTS telemetry (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            device_id TEXT NOT NULL,
            seq INTEGER NOT NULL,
            timestamp INTEGER NOT NULL,
            pm1_0 REAL,
            pm2_5 REAL,
            pm10 REAL,
            co2_ppm REAL,
            temperature_c REAL,
            humidity_rh REAL,
            aqi INTEGER,
            battery_pct INTEGER,
            network_state TEXT,
            valid_pm INTEGER,
            valid_co2 INTEGER,
            valid_trh INTEGER,
            raw_json TEXT NOT NULL
        )
    """)
    conn.commit()
    return conn

def insert_telemetry(conn, record: Dict[str, Any]):
    cur = conn.cursor()
    s = record.get("sensors", {})
    v = record.get("validity", {})
    diag = record.get("diagnostics", {})

    cur.execute("""
        INSERT INTO telemetry (
            device_id, seq, timestamp,
            pm1_0, pm2_5, pm10, co2_ppm,
            temperature_c, humidity_rh, aqi,
            battery_pct, network_state,
            valid_pm, valid_co2, valid_trh,
            raw_json
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    """, (
        record.get("device_id", "unknown"),
        record.get("seq", 0),
        record.get("timestamp", 0),
        s.get("pm1_0"),
        s.get("pm2_5"),
        s.get("pm10"),
        s.get("co2_ppm"),
        s.get("temperature_c"),
        s.get("humidity_rh"),
        s.get("aqi"),
        diag.get("battery_pct"),
        diag.get("network_state", "CONNECTED"),
        1 if v.get("pm", True) else 0,
        1 if v.get("co2", True) else 0,
        1 if v.get("trh", True) else 0,
        json.dumps(record)
    ))
    conn.commit()

def main():
    parser = argparse.ArgumentParser(description="Air Monitor Telemetry Receiver Daemon")
    parser.add_argument("--db", default=DEFAULT_DB_PATH, help="Path to SQLite database file")
    parser.add_argument("--file", default=None, help="Input JSONL file to ingest (reads stdin if not specified)")
    args = parser.parse_args()

    conn = init_db(args.db)
    print(f"[RECEIVER] Telemetry database initialized: {args.db}")

    count = 0
    in_stream = open(args.file, "r", encoding="utf-8") if args.file else sys.stdin

    try:
        for line in in_stream:
            line = line.strip()
            if not line or not line.startswith("{"):
                continue
            try:
                record = json.loads(line)
                insert_telemetry(conn, record)
                count += 1
                print(f"[INGEST {count}] Device:{record.get('device_id')} Seq:{record.get('seq')} "
                      f"PM2.5:{record.get('sensors', {}).get('pm2_5')} AQI:{record.get('sensors', {}).get('aqi')}")
            except json.JSONDecodeError:
                pass
    except KeyboardInterrupt:
        pass
    finally:
        if args.file:
            in_stream.close()
        conn.close()

    print(f"[RECEIVER] Session closed. Ingested {count} records into {args.db}")

if __name__ == "__main__":
    main()
