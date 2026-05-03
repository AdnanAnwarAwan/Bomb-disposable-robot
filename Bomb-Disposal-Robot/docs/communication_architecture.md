# Communication Architecture

## Tri-Level Hybrid System

| Level | Module | Range | Primary Use |
|---|---|---|---|
| 1 | HC-05 Bluetooth | ~100m | Calibration, pre-flight |
| 2 | ESP32 Wi-Fi | ~300m | FPV video + primary control |
| 3 | GSM/DTMF | Global | BVLOS long-range |

## DTMF Command Map (MT8870)

| Key | Binary | Action |
|---|---|---|
| 2 | 0010 | Move Forward (500ms burst) |
| 4 | 0100 | Turn Left |
| 5 | 0101 | Stop all |
| 6 | 0110 | Turn Right |
| 8 | 1000 | Move Backward |
| 1 | 0001 | Arm Base CW |
| 3 | 0011 | Arm Base CCW |
| 7 | 0111 | Shoulder Up |
| 9 | 1001 | Shoulder Down |
| 0 | 1010 | Gripper Open |
| * | 1011 | Gripper Close |
| # | 1100 | Emergency Stop |

## Auto-Failover State Machine

```
BLUETOOTH_CONNECTED
  → BT lost 2s → WIFI_MODE
      → WiFi lost 2s → GSM_MODE
          → Heartbeat lost 2s → SAFE_MODE (all stop)
```

## Firebase Realtime Database Schema

```json
{
  "control": {
    "heartbeat": 1234567890,
    "move": "F",
    "arm_base": 90,
    "arm_shoulder": 45,
    "arm_elbow": 90,
    "arm_gripper": 0,
    "estop": false
  },
  "telemetry": {
    "battery_v": 11.4,
    "signal_rssi": -67,
    "gps_lat": 0.0,
    "gps_lon": 0.0,
    "imu_roll": 2.1,
    "imu_pitch": -1.3,
    "latency_ms": 145,
    "mode": "WIFI"
  }
}
```

## Latency Monitoring

- GCS sends command with timestamp
- Robot echoes ACK with same timestamp
- GCS shows round-trip latency on UI
- > 300ms → yellow warning
- > 500ms → red warning + auto-reduce speed
