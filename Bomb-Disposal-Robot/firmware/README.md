# Firmware

## Build Requirements
- Arduino IDE 2.x or VS Code + Arduino extension
- Board: Arduino Mega 2560
- Libraries: `Servo` (built-in), `Wire` (built-in)
- Programmer: USB-B cable to PC

## File Structure

```
Core/Src/
  main.ino          Main sketch — command router, watchdog, telemetry
  arm_control.cpp   4-DOF servo control with joint limits
  drive_control.cpp BTS7960 H-bridge, command-pulse strategy
  dtmf_decoder.cpp  MT8870 DTMF to action decoder
  watchdog.cpp      Dead-man switch (2s timeout)
  imu.cpp           MPU-6050 tip-over detection

Core/Inc/
  arm_control.h
  drive_control.h
  dtmf_decoder.h
  watchdog.h
  imu.h
```

## Upload Steps

1. Open `firmware/Core/Src/main.ino` in Arduino IDE
2. Select board: **Arduino Mega 2560**
3. Select correct COM port
4. Click **Upload**

## Serial Command Protocol (Bluetooth + Wi-Fi share same format)

| Command | Example | Action |
|---|---|---|
| `F<ms>` | `F500` | Forward 500ms |
| `B<ms>` | `B500` | Backward 500ms |
| `L<ms>` | `L500` | Left 500ms |
| `R<ms>` | `R500` | Right 500ms |
| `S` | `S` | Stop |
| `A<deg>` | `A090` | Base to 90° |
| `C<deg>` | `C045` | Shoulder to 45° |
| `E<deg>` | `E120` | Elbow to 120° |
| `G<deg>` | `G000` | Gripper open |
| `H` | `H` | Heartbeat kick |
| `X` | `X` | Emergency stop |

## Tuning Parameters

| Constant | File | Default | Effect |
|---|---|---|---|
| `PULSE_MS` | drive_control.cpp | 500 | DTMF movement burst duration |
| `DRIVE_SPEED` | drive_control.cpp | 180 | Motor PWM (0–255) |
| `SERVO_STEP_MS` | arm_control.cpp | 15 | Servo smoothing speed |
| `WATCHDOG_TIMEOUT_MS` | watchdog.cpp | 2000 | Dead-man switch timeout |
| `TIPOVER_ROLL_STOP` | imu.cpp | 45.0 | Roll tip-over threshold |
