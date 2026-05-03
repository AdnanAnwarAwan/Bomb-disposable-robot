# System Overview

## Mission Profile

Designed for **Explosive Ordnance Disposal (EOD)** where:
- Device is beyond safe human approach distance
- Terrain is uneven (debris, rubble)
- Operation must continue beyond visual line of sight (BVLOS)

## Industrial Upgrades Over Original Design

| Area | Hobby grade (original) | Industrial grade (this repo) |
|---|---|---|
| Communication | Single Bluetooth | Tri-level BT/WiFi/GSM auto-failover |
| Safety | None | Dead-man switch + tip-over + brownout |
| Power | Single LiPo rail | Dual-rail isolated (logic + motor) |
| EMI | None | Snubber caps + shielded cables |
| Control | Basic on/off | Command-pulse + latency monitoring |
| Video | None | ESP32-CAM MJPEG + WebRTC |
| Firmware | Simple loop | ISR-driven + hardware watchdog |
| Telemetry | None | Firebase + GPS + battery + signal |

## Signal Flow

```
Operator (Android GCS)
    │
    ├─ Bluetooth HC-05 ──── <100m calibration
    ├─ Wi-Fi ESP32 ───────── <300m FPV + control
    └─ GSM DTMF ─────────── Global BVLOS
              │
              ▼
      MT8870 DTMF Decoder (4-bit binary)
              │
              ▼
      Arduino Mega 2560
       ├── Servo PID (4-DOF arm)
       ├── Motor PWM (BTS7960 drive)
       ├── Safety watchdog
       ├── IMU tip-over detection
       └── Telemetry → Firebase
              │
    ┌─────────┴──────────┐
  4-DOF Arm          Tracked Drive
  MG996R × 4         BTS7960 + DC motors
```
