# Safety Systems

## 1. Dead Man's Switch
- GCS sends Firebase heartbeat every 500ms
- Robot checks every 100ms
- If heartbeat not updated for 2000ms → Safe Mode
- Safe Mode: all motors stop, arm retracts to safe position

## 2. Tip-Over Detection (MPU-6050)
```
Roll  > 35° → GCS warning
Roll  > 45° → ARM RETRACT + DRIVE STOP
Pitch > 30° → GCS warning
Pitch > 40° → ARM RETRACT + DRIVE STOP

Safe arm position: base=0°, shoulder=45°, elbow=90°, gripper=open
Resume: automatic when IMU reads < 20° for 3 consecutive seconds
```

## 3. Brownout Protection
- Dedicated LDO 5V rail for logic (isolated from motor rail)
- 2200µF bulk cap holds up through 2.2ms motor transients

## 4. EMI Hardening
- 100nF ceramic cap across each motor terminal
- Shielded cable for DTMF audio path
- MT8870 StD pin used as Arduino external interrupt (rejects noise)
- GSM antenna > 15cm from Wi-Fi antenna

## 5. Command-Pulse Strategy
- Each DTMF keypress → exactly 500ms movement burst → stop
- Forces operator to pulse controls (syncs with video lag)
- Prevents over-steering at 300ms+ GSM latency

## 6. Emergency Stop (Key #)
1. Motor PWM → 0 immediately
2. Arm servos hold position
3. Firebase ESTOP flag → true
4. Robot chassis LED → red

## 7. Battery Low Cutoff
- < 10.5V → GCS yellow warning
- < 9.9V → arm retract + movement stop + GCS red alert
