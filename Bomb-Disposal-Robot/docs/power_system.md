# Power System Design

## Dual-Rail Architecture

```
LiPo 3S (11.1V nominal)
    │
    ├─── LM2596 Buck #1 → 5V/3A (Logic rail)
    │         │
    │         ├── 2200µF bulk cap (brownout buffer)
    │         ├── Arduino Mega
    │         ├── HC-05 Bluetooth
    │         └── MT8870 DTMF
    │
    ├─── LM2596 Buck #2 → 6V/5A (Servo rail)
    │         │
    │         ├── 1000µF bulk cap
    │         └── MG996R × 5 servos
    │
    └─── BTS7960 H-Bridge (direct battery)
              │
              └── 2× DC gear motors (high current, isolated)
```

## Power Budget

| Load | Voltage | Avg | Peak |
|---|---|---|---|
| Arduino Mega | 5V | 80mA | 200mA |
| HC-05 BT | 3.3V | 30mA | 80mA |
| MT8870 | 5V | 15mA | 15mA |
| ESP32-CAM | 3.3V | 200mA | 500mA |
| 5× MG996R | 6V | 1.25A | 12.5A |
| 2× Drive motors | 11.1V | 2A | 10A |
| **Total** | | **~3.8A** | **~23A** |

## Battery Specification

- Chemistry: LiPo 3S (11.1V nominal, 12.6V full, 9.9V cutoff)
- Capacity: 5000mAh
- Discharge rating: 30C minimum
- Estimated runtime: ~45 min (with 40% safety margin)

## EMI Suppression

```
Each motor: 100nF ceramic cap across terminals + 1N4007 flyback diode
DTMF path: Shielded aux cable from phone headphone jack to MT8870
Antenna separation: GSM antenna >15cm from ESP32 Wi-Fi antenna
```

## Brownout Hold-up Calculation

```
2200µF capacitor on 5V rail
ΔV allowed: 0.5V
Load during sag: 0.5A (Arduino + peripherals)
Hold-up time: C × ΔV / I = 0.0022 × 0.5 / 0.5 = 2.2ms ✅
(LM2596 recovers within 1ms of transient)
```
