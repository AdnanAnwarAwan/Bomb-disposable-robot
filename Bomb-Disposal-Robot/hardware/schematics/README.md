# Schematics

## System Schematic Overview

Key connections:
- MT8870 Q1-Q4 → Arduino Mega D4-D7
- MT8870 StD → Arduino D2 (external interrupt)
- HC-05 TX/RX → Arduino Serial1 (D18/D19)
- ESP32 TX/RX → Arduino Serial2 (D16/D17)
- MPU-6050 SDA/SCL → Arduino SDA/SCL (D20/D21)
- MG996R servos → Arduino D22, D23, D24, D25
- BTS7960 Left: RPWM=D8, LPWM=D9
- BTS7960 Right: RPWM=D10, LPWM=D11
- Battery voltage divider → A0

## DTMF Audio Path

```
Android phone headphone jack
    │ (3.5mm shielded aux cable)
    ▼
MT8870 input (pins 2, 3)
    │ RC filter (10kΩ + 100nF) for noise rejection
    ▼
MT8870 Q1-Q4 outputs → Arduino D4-D7
MT8870 StD (Delayed Steering) → Arduino D2 (interrupt)
```
