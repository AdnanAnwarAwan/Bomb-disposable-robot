# Bomb Disposal Robotic Arm Vehicle
### Industrial-Grade BVLOS EOD Robot | Arduino Mega + ESP32 + GSM/DTMF | 4-DOF Arm

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![MCU](https://img.shields.io/badge/MCU-Arduino%20Mega%202560-brightgreen)
![Comms](https://img.shields.io/badge/Comms-Bluetooth%20%7C%20WiFi%20%7C%20GSM-orange)
![Control](https://img.shields.io/badge/Control-Android%20GCS-red)
![DOF](https://img.shields.io/badge/Arm-4%20DOF-purple)

---

## Overview

An industrial-grade **Explosive Ordnance Disposal (EOD)** robotic system designed for **Beyond Visual Line of Sight (BVLOS)** operations. The system integrates a 4-DOF robotic arm, tracked locomotion, and a tri-level hybrid communication architecture (Bluetooth / Wi-Fi / GSM-DTMF) controlled via an Android Ground Control Station (GCS).

This project covers the complete engineering lifecycle from concept to deployment:
- **Mechanical design** — 4-DOF manipulator with torque-calculated servo selection
- **Power electronics** — Dual-rail isolation, brownout protection, EMI suppression
- **Embedded firmware** — Arduino Mega, DTMF decoding, safety watchdog, PID servo control
- **Wireless architecture** — Tri-level Bluetooth/Wi-Fi/GSM with automatic failover
- **Android GCS** — MIT App Inventor with Firebase Realtime Database and WebRTC video
- **Safety systems** — Dead man's switch, heartbeat watchdog, safe-mode fallback

---

## System Specifications

| Parameter | Value |
|---|---|
| Arm DOF | 4 (Base, Shoulder, Elbow, Gripper) |
| Arm payload capacity | ~500g at full extension |
| Locomotion | Tracked (rubber track, 2× DC gear motor) |
| Control range (Bluetooth) | ~100m |
| Control range (Wi-Fi) | ~300m |
| Control range (GSM) | Global (BVLOS) |
| Video stream | MJPEG via ESP32-CAM (720p) |
| Communication latency | <300ms GSM, <50ms Wi-Fi |
| Dead-man switch timeout | 2 seconds |
| Battery | 11.1V 5000mAh LiPo (3S) |
| Operating time | ~45 minutes full operation |
| Main MCU | Arduino Mega 2560 |
| Co-processor | ESP32 (Wi-Fi + camera) |

---

## System Architecture

```
┌─────────────────────────────────────────────────────────┐
│                 ANDROID GCS (Ground Control)             │
│  MIT App Inventor  │  Firebase DB  │  WebRTC/RTSP Video  │
└──────────────┬──────────────┬───────────────────────────┘
               │              │
    ┌──────────▼──────┐  ┌────▼──────────┐
    │  Bluetooth HC-05 │  │  Wi-Fi ESP32  │
    │  (0-100m local) │  │  (0-300m FPV) │
    └──────────┬───────┘  └────┬──────────┘
               │               │
    ┌──────────▼───────────────▼──────────────┐
    │           Arduino Mega 2560              │
    │  • DTMF decode (MT8870)                  │
    │  • Servo PID (4-DOF arm)                 │
    │  • Motor PWM (H-bridge BTS7960)          │
    │  • Safety watchdog (dead-man switch)     │
    │  • Power management                      │
    └──────────────────┬──────────────────────┘
                       │
         ┌─────────────┴──────────────┐
         │                            │
    ┌────▼──────┐              ┌──────▼─────┐
    │  4-DOF    │              │  Tracked   │
    │  Arm      │              │  Drive     │
    │  MG996R×4 │              │  BTS7960×2 │
    └───────────┘              └────────────┘

GSM Path (BVLOS):
  Android Phone → GSM Call → Robot Phone → MT8870 DTMF Decoder → Arduino
```

---

## Hardware Components

| Component | Part | Purpose |
|---|---|---|
| Main MCU | Arduino Mega 2560 | Central control, servo PWM, DTMF decode |
| Wi-Fi / Camera | ESP32-CAM | FPV video stream + Wi-Fi control |
| DTMF Decoder | MT8870 | Decode GSM phone tones to 4-bit binary |
| Motor Driver (drive) | BTS7960 | High-current H-bridge for drive motors |
| Motor Driver (arm) | L298N | Servo power distribution |
| Arm Servos | MG996R ×4 | Base, shoulder, elbow, gripper |
| Drive Motors | JGA25-370 | High-torque 12V DC gear motors |
| Bluetooth | HC-05 | Short-range UART control |
| Cellular | On-board Android phone | GSM DTMF BVLOS control |
| IMU | MPU-6050 | Tip-over detection, stabilisation |
| Battery | 3S LiPo 5000mAh | Main power |
| Step-down 1 | LM2596 (5V/3A) | Logic power rail |
| Step-down 2 | LM2596 (6V/5A) | Servo power rail |
| Bulk cap | 2200µF/25V | Brownout protection |
| Snubber caps | 100nF ceramic ×4 | Motor EMI suppression |

---

## Communication Architecture — Tri-Level Hybrid

| Mode | Module | Range | Use case |
|---|---|---|---|
| Bluetooth | HC-05 | ~100m | Calibration, pre-flight checks |
| Wi-Fi | ESP32 | ~300m | High-bandwidth FPV + control |
| GSM/DTMF | Android phone | Global | Primary BVLOS link |

Auto-failover logic: Wi-Fi → GSM when signal drops below threshold.

---

## Safety Systems

- **Dead man's switch** — Firebase heartbeat every 500ms; robot enters safe mode if not updated for 2s
- **Tip-over detection** — MPU-6050 IMU monitors roll/pitch; arm auto-retracts if threshold exceeded
- **Brownout protection** — Dedicated LDO rail for logic with 2200µF bulk capacitor
- **EMI suppression** — 100nF ceramic caps across all DC motor terminals
- **DTMF interrupt guard** — MT8870 StD pin used as Arduino interrupt to reject noise-induced ghost movements
- **Command-pulse strategy** — Each DTMF keypress triggers discrete 500ms movement burst (prevents over-steering)

---

## Repository Structure

```
Bomb-Disposal-Robot/
├── README.md
├── LICENSE
├── .gitignore
├── docs/
│   ├── system_overview.md
│   ├── communication_architecture.md
│   ├── arm_design_calculations.md
│   ├── power_system.md
│   ├── safety_systems.md
│   └── test_procedure.md
├── hardware/
│   ├── schematics/          Full circuit schematic
│   ├── bom/BOM.csv          35+ component BOM
│   └── pcb/                 PCB layout notes
├── firmware/
│   ├── Core/Src/
│   │   ├── main.ino         Main Arduino sketch
│   │   ├── arm_control.cpp  4-DOF PID servo control
│   │   ├── drive_control.cpp Tracked locomotion
│   │   ├── dtmf_decoder.cpp  MT8870 DTMF interface
│   │   ├── watchdog.cpp      Dead-man switch safety
│   │   └── imu.cpp          MPU-6050 tip-over detection
│   ├── Core/Inc/
│   │   └── *.h header files
│   └── README.md
├── software/
│   ├── android_app/         MIT App Inventor .aia source
│   └── firebase/            Firebase rules + schema
└── presentation/
    └── README.md
```

---

## Industrial Upgrade from Original Design

| Feature | Original (Arduino hobby) | Industrial grade (this repo) |
|---|---|---|
| Communication | Single Bluetooth | Tri-level BT/WiFi/GSM with auto-failover |
| Safety | None | Dead-man switch + tip-over + brownout protection |
| Power | Single rail | Dual-rail isolated (logic + motor) |
| EMI | None | Snubber caps + shielded cables + domain separation |
| Control | Basic on/off | Command-pulse with latency monitoring |
| Video | None | ESP32-CAM MJPEG + WebRTC |
| Firmware | Simple loop | ISR-driven, watchdog-protected |
| Telemetry | None | Firebase + GPS + battery voltage + signal strength |

---

## License

MIT License — see [LICENSE](LICENSE)

## Author

**Adnan Anwar Awan**
Electrical Engineer — Robotics | Embedded Systems | Wireless Communication
GitHub: [@AdnanAnwarAwan](https://github.com/AdnanAnwarAwan)
