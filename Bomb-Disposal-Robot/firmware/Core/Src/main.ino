/**
 * @file    main.ino
 * @brief   Bomb Disposal Robotic Arm Vehicle — Main Firmware
 *          Industrial-grade BVLOS EOD Robot
 *
 * Hardware:
 *   MCU:       Arduino Mega 2560
 *   Arm:       4× MG996R servo (Base, Shoulder, Elbow, Gripper)
 *   Drive:     2× BTS7960 H-bridge + DC gear motors
 *   DTMF:      MT8870 decoder (GSM BVLOS control)
 *   BT:        HC-05 (Serial1, 9600 baud)
 *   WiFi:      ESP32 (Serial2, 115200 baud)
 *   IMU:       MPU-6050 (I2C, addr 0x68)
 *   Battery:   Voltage divider on A0
 *
 * Pin map:
 *   D2   MT8870 StD interrupt (valid DTMF detected)
 *   D4   MT8870 Q1
 *   D5   MT8870 Q2
 *   D6   MT8870 Q3
 *   D7   MT8870 Q4
 *   D8   BTS7960 Left  RPWM
 *   D9   BTS7960 Left  LPWM
 *   D10  BTS7960 Right RPWM
 *   D11  BTS7960 Right LPWM
 *   D22  Servo Base    (PWM via Servo lib)
 *   D23  Servo Shoulder
 *   D24  Servo Elbow
 *   D25  Servo Gripper
 *   D13  Status LED
 *   A0   Battery voltage divider (R1=10k, R2=3.3k → 0-15V → 0-5V)
 *
 * @author  Adnan Anwar Awan
 */

#include <Servo.h>
#include <Wire.h>
#include "arm_control.h"
#include "drive_control.h"
#include "dtmf_decoder.h"
#include "watchdog.h"
#include "imu.h"

// ── Pin definitions ────────────────────────────────────────────────────────
#define PIN_DTMF_STD    2       // MT8870 StD — external interrupt
#define PIN_DTMF_Q1     4
#define PIN_DTMF_Q2     5
#define PIN_DTMF_Q3     6
#define PIN_DTMF_Q4     7
#define PIN_BATT_ADC    A0
#define PIN_STATUS_LED  13

// ── Battery thresholds ─────────────────────────────────────────────────────
#define BATT_WARN_V     10.5f   // Yellow warning
#define BATT_CUTOFF_V   9.9f    // Red alert + auto-stop

// ── Globals ────────────────────────────────────────────────────────────────
static bool estop_active   = false;
static bool batt_cutoff    = false;
static uint32_t last_batt_check = 0;
static uint32_t last_telem      = 0;

// ── DTMF ISR flag ─────────────────────────────────────────────────────────
volatile bool dtmf_received = false;
volatile uint8_t dtmf_value = 0;

void dtmf_isr(void) {
    // Read Q1-Q4 from MT8870 output pins
    uint8_t q1 = digitalRead(PIN_DTMF_Q1);
    uint8_t q2 = digitalRead(PIN_DTMF_Q2);
    uint8_t q3 = digitalRead(PIN_DTMF_Q3);
    uint8_t q4 = digitalRead(PIN_DTMF_Q4);
    dtmf_value   = (q4 << 3) | (q3 << 2) | (q2 << 1) | q1;
    dtmf_received = true;
}

// ──────────────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    Serial1.begin(9600);     // HC-05 Bluetooth
    Serial2.begin(115200);   // ESP32 WiFi

    // Pin modes
    pinMode(PIN_DTMF_STD, INPUT);
    pinMode(PIN_DTMF_Q1,  INPUT);
    pinMode(PIN_DTMF_Q2,  INPUT);
    pinMode(PIN_DTMF_Q3,  INPUT);
    pinMode(PIN_DTMF_Q4,  INPUT);
    pinMode(PIN_STATUS_LED, OUTPUT);

    // Attach DTMF interrupt (rising edge = valid tone detected)
    attachInterrupt(digitalPinToInterrupt(PIN_DTMF_STD), dtmf_isr, RISING);

    // Subsystem init
    ARM_Init();
    DRIVE_Init();
    IMU_Init();
    WATCHDOG_Init();

    // Move arm to safe start position
    ARM_SetSafePosition();

    Serial.println(F("=== Bomb Disposal Robot ONLINE ==="));
    Serial.println(F("Waiting for commands..."));
    digitalWrite(PIN_STATUS_LED, HIGH);
}

// ──────────────────────────────────────────────────────────────────────────
void loop() {
    uint32_t now = millis();

    // ── 1. Safety watchdog check (every 100ms) ──────────────────────────
    if (WATCHDOG_IsExpired()) {
        if (!estop_active) {
            Serial.println(F("WATCHDOG: Heartbeat lost — Safe Mode"));
            DRIVE_Stop();
            ARM_SetSafePosition();
            estop_active = true;
        }
    } else {
        estop_active = false;
    }

    // ── 2. Tip-over check ───────────────────────────────────────────────
    if (IMU_TipOverDetected()) {
        Serial.println(F("IMU: Tip-over! Retracting arm."));
        DRIVE_Stop();
        ARM_SetSafePosition();
    }

    // ── 3. DTMF command processing ──────────────────────────────────────
    if (dtmf_received && !estop_active && !batt_cutoff) {
        dtmf_received = false;
        DTMF_ProcessCommand(dtmf_value);
        WATCHDOG_Kick();    // Valid command = heartbeat proof
    }

    // ── 4. Bluetooth command processing ─────────────────────────────────
    if (Serial1.available() && !estop_active && !batt_cutoff) {
        String cmd = Serial1.readStringUntil('\n');
        cmd.trim();
        ProcessSerialCommand(cmd);
        WATCHDOG_Kick();
    }

    // ── 5. ESP32 WiFi command processing ────────────────────────────────
    if (Serial2.available() && !estop_active && !batt_cutoff) {
        String cmd = Serial2.readStringUntil('\n');
        cmd.trim();
        ProcessSerialCommand(cmd);
        WATCHDOG_Kick();
    }

    // ── 6. Battery monitoring (every 5s) ────────────────────────────────
    if (now - last_batt_check >= 5000) {
        last_batt_check = now;
        float batt_v = ReadBatteryVoltage();
        if (batt_v < BATT_CUTOFF_V) {
            if (!batt_cutoff) {
                Serial.print(F("BATT CRITICAL: "));
                Serial.println(batt_v);
                DRIVE_Stop();
                ARM_SetSafePosition();
                batt_cutoff = true;
            }
        } else if (batt_v < BATT_WARN_V) {
            Serial.print(F("BATT LOW: "));
            Serial.println(batt_v);
        }
    }

    // ── 7. Telemetry output (every 500ms) ────────────────────────────────
    if (now - last_telem >= 500) {
        last_telem = now;
        float batt_v = ReadBatteryVoltage();
        float roll   = IMU_GetRoll();
        float pitch  = IMU_GetPitch();
        // Send to ESP32 for Firebase upload
        Serial2.print(F("TEL:BAT="));   Serial2.print(batt_v, 1);
        Serial2.print(F(",ROL="));      Serial2.print(roll, 1);
        Serial2.print(F(",PIT="));      Serial2.print(pitch, 1);
        Serial2.println();
    }
}

// ── Serial command parser (Bluetooth + WiFi share same protocol) ───────────
void ProcessSerialCommand(const String &cmd) {
    if (cmd.length() < 1) return;

    char code = cmd.charAt(0);
    int  val  = cmd.substring(1).toInt();

    switch (code) {
        case 'F': DRIVE_Forward(val > 0 ? val : 500);  break;
        case 'B': DRIVE_Backward(val > 0 ? val : 500); break;
        case 'L': DRIVE_Left(val > 0 ? val : 500);     break;
        case 'R': DRIVE_Right(val > 0 ? val : 500);    break;
        case 'S': DRIVE_Stop();                          break;
        case 'A': ARM_SetBase(val);                      break;  // A090
        case 'C': ARM_SetShoulder(val);                  break;  // C045
        case 'E': ARM_SetElbow(val);                     break;  // E090
        case 'G': ARM_SetGripper(val);                   break;  // G000=open
        case 'X': DRIVE_Stop(); ARM_SetSafePosition();   break;  // ESTOP
        case 'H': WATCHDOG_Kick();                       break;  // Heartbeat
        default:  break;
    }
}

// ── Battery voltage reading ────────────────────────────────────────────────
float ReadBatteryVoltage() {
    int raw = analogRead(PIN_BATT_ADC);
    // Voltage divider: R1=10k, R2=3.3k → Vbatt = Vadc × (10+3.3)/3.3
    float v_adc = (raw / 1023.0f) * 5.0f;
    return v_adc * (13.3f / 3.3f);
}
