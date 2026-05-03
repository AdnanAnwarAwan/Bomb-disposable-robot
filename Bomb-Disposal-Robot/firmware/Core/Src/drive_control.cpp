/**
 * @file    drive_control.cpp
 * @brief   Tracked locomotion via BTS7960 H-bridge
 *          Command-pulse strategy: each command triggers timed burst then stops
 *
 * BTS7960 pin map:
 *   Left  motor: RPWM=D8, LPWM=D9
 *   Right motor: RPWM=D10, LPWM=D11
 *
 * @author  Adnan Anwar Awan
 */

#include "drive_control.h"
#include <Arduino.h>

#define PIN_L_RPWM  8
#define PIN_L_LPWM  9
#define PIN_R_RPWM  10
#define PIN_R_LPWM  11

#define DRIVE_SPEED     180     // 0-255 PWM
#define TURN_SPEED      150
#define PULSE_MS        500     // Command-pulse duration (BVLOS over-steer prevention)

void DRIVE_Init() {
    pinMode(PIN_L_RPWM, OUTPUT);
    pinMode(PIN_L_LPWM, OUTPUT);
    pinMode(PIN_R_RPWM, OUTPUT);
    pinMode(PIN_R_LPWM, OUTPUT);
    DRIVE_Stop();
}

static void setMotors(int l_rpwm, int l_lpwm, int r_rpwm, int r_lpwm) {
    analogWrite(PIN_L_RPWM, l_rpwm);
    analogWrite(PIN_L_LPWM, l_lpwm);
    analogWrite(PIN_R_RPWM, r_rpwm);
    analogWrite(PIN_R_LPWM, r_lpwm);
}

// duration_ms = 0 → continuous (Wi-Fi mode), > 0 → pulse (DTMF/BVLOS mode)
void DRIVE_Forward(uint32_t duration_ms) {
    setMotors(DRIVE_SPEED, 0, DRIVE_SPEED, 0);
    if (duration_ms > 0) { delay(duration_ms); DRIVE_Stop(); }
}

void DRIVE_Backward(uint32_t duration_ms) {
    setMotors(0, DRIVE_SPEED, 0, DRIVE_SPEED);
    if (duration_ms > 0) { delay(duration_ms); DRIVE_Stop(); }
}

void DRIVE_Left(uint32_t duration_ms) {
    setMotors(0, TURN_SPEED, TURN_SPEED, 0);    // Left track back, right forward
    if (duration_ms > 0) { delay(duration_ms); DRIVE_Stop(); }
}

void DRIVE_Right(uint32_t duration_ms) {
    setMotors(TURN_SPEED, 0, 0, TURN_SPEED);    // Right track back, left forward
    if (duration_ms > 0) { delay(duration_ms); DRIVE_Stop(); }
}

void DRIVE_Stop() {
    setMotors(0, 0, 0, 0);
}
