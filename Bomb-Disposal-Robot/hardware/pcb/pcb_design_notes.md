# PCB / Wiring Design Notes

## Power Distribution Board

A dedicated power distribution board (PDB) is recommended:

```
LiPo XT60 IN
    │
    ├── Main fuse (30A blade fuse)
    ├── BTS7960 #1 (Left drive motor) — direct battery
    ├── BTS7960 #2 (Right drive motor) — direct battery
    ├── LM2596 #1 → 5V logic rail
    └── LM2596 #2 → 6V servo rail
```

## Critical Wiring Rules

- Motor wires: minimum 18AWG, keep short
- Logic wires: 22–26AWG
- Servo wires: 20AWG minimum (each servo peaks at 2.5A)
- All motor terminals: 100nF ceramic cap soldered directly across terminals
- DTMF audio cable: shielded 3.5mm aux, kept away from motor wiring
- Battery leads: 12AWG silicon wire minimum

## Antenna Placement

- GSM phone antenna: mounted at rear of chassis, as high as possible
- ESP32 antenna: mounted at front of chassis
- Minimum separation: 150mm between GSM and Wi-Fi antennas

## Ground Strategy

- Single ground star point at LiPo negative terminal
- Logic ground and motor ground meet only at battery negative
- No motor return currents through logic PCB
