# Test & Validation Procedure

## Equipment
- Android GCS phone
- On-board robot phone (GSM)
- Multimeter, tape measure
- Oscilloscope (optional)

## Step 1 — Power Rail Verification
- [ ] 5V logic rail: **4.75–5.25V** ✅
- [ ] 6V servo rail: **5.7–6.3V** ✅
- [ ] Arduino boots, serial shows startup message
- [ ] No brownout resets during motor stall

## Step 2 — Servo Calibration
- [ ] Each servo: 0°, 90°, 180° — smooth, no jitter ✅
- [ ] Gripper 10 open/close cycles — no drift ✅

## Step 3 — Drive Motor Test
- [ ] Forward/Reverse/Left/Right — correct directions ✅
- [ ] Emergency stop: < 100ms ✅
- [ ] No Arduino reset at full-throttle start ✅

## Step 4 — DTMF Decoding
- [ ] All 12 keys decode correctly on Serial monitor ✅
- [ ] Key # triggers emergency stop ✅
- [ ] StD interrupt rejects noise (no ghost movements) ✅

## Step 5 — Wi-Fi Range
- [ ] 50m: responsive + video ✅
- [ ] 150m: responsive ✅
- [ ] 300m: responsive (may degrade) ✅

## Step 6 — GSM BVLOS Test
- [ ] Call on-board phone — DTMF control works ✅
- [ ] > 500m range: robot responds ✅
- [ ] Latency: **< 500ms** ✅

## Step 7 — Dead Man's Switch
- [ ] Kill GCS app → robot stops within **2 seconds** ✅
- [ ] Restart GCS → normal operation resumes ✅

## Step 8 — Tip-Over Detection
- [ ] Tilt 35°: GCS warning appears ✅
- [ ] Tilt 45°: arm retracts + drive stops ✅
- [ ] Level robot: resumes after 3 seconds ✅

## Step 9 — Payload Test
- [ ] 300g at full extension: arm holds steady ✅
- [ ] 500g: arm holds (rated limit) ✅

## Step 10 — Full Mission Simulation
- [ ] Navigate 50m via BVLOS only ✅
- [ ] Pick up and relocate target object ✅
- [ ] Zero resets or faults during full run ✅
- [ ] End battery > 10.5V ✅
