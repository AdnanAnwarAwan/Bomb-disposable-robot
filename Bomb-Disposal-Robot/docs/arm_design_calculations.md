# Robotic Arm Design Calculations

## Degrees of Freedom (DOF)

| Joint | Range | Actuator |
|---|---|---|
| Base rotation | 0–360° | MG996R servo |
| Shoulder | 0–180° | 2× MG996R (parallel) |
| Elbow | 0–180° | MG996R servo |
| Gripper | 0–90° | MG996R servo |
| **Total DOF** | **4** | |

## Shoulder Torque Calculation (Worst Case)

```
Effective mass at shoulder pivot:
  Forearm + gripper + payload = 0.8 kg
  Upper arm (half) = 0.2 kg
  Total effective mass = 1.0 kg

Max extension moment arm: L = 0.35m

Required torque:
  T = m × g × L = 1.0 × 9.81 × 0.35 = 3.43 N·m = 343 N·cm

MG996R stall torque @ 6V: ~11 kg·cm = 108 N·cm
Safety factor: 2×
Servos needed: 343 / (108/2) = 6.4 → use 2× parallel = 216 N·cm usable

Conclusion: Double shoulder servos, limit extension to 25cm max
```

## Gripper Force

```
Required: hold 500g object with 3× safety factor
F_grip = 0.5 × 9.81 × 3 = 14.7 N

MG996R torque: 108 N·cm, jaw moment arm: 2cm
Available grip force: 108/2 = 54 N ✅ (3.7× margin)
```

## MG996R Servo Specifications

| Parameter | Value |
|---|---|
| Stall torque | 11 kg·cm @ 6V |
| Speed | 0.17 sec/60° @ 6V |
| Operating voltage | 4.8–7.2V |
| Stall current | 2.5A |
| PWM | 50Hz, 1–2ms pulse |
