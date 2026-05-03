# Android GCS App (MIT App Inventor)

## Overview

The Ground Control Station (GCS) app is built with MIT App Inventor.
Source file: `BombDisposalGCS.aia` (import into appinventor.mit.edu)

## Components Used

| Component | Purpose |
|---|---|
| FirebaseDB | Send control commands, read telemetry |
| WebViewer | Display ESP32-CAM live video stream |
| Clock (500ms) | Send heartbeat + read telemetry |
| Slider × 4 | Arm joint control (Base, Shoulder, Elbow, Gripper) |
| Button × 5 | D-Pad for drive (F/B/L/R/Stop) |
| Label × 6 | Battery, signal, latency, mode, GPS, status display |

## UI Layout

```
┌──────────────────────────────────────────┐
│  Battery: 11.4V  Signal: -67dBm  Lat:145ms │
├──────────────────────────────────────────┤
│                                          │
│         WebViewer (70% height)           │
│         ESP32-CAM live feed              │
│                                          │
├───────────────────┬──────────────────────┤
│    D-Pad          │   Arm Sliders        │
│  [FWD]            │   Base:     ──●──    │
│[LFT][STP][RGT]    │   Shoulder: ──●──    │
│  [BCK]            │   Elbow:    ──●──    │
│                   │   Gripper:  ──●──    │
│  [ESTOP] (red)    │                      │
└───────────────────┴──────────────────────┘
```

## Heartbeat Logic (Blocks)

```
When Clock.Timer fires:
  Call FirebaseDB.StoreValue(tag="control/heartbeat", value=Clock.Now)
  Call FirebaseDB.GetValue(tag="telemetry/battery_v")
  Call FirebaseDB.GetValue(tag="telemetry/latency_ms")
```

## Connection Modes

Set Firebase project URL in Screen1.Initialize.
Toggle between BT/WiFi/GSM using mode selector buttons at top of screen.
