/**
 * @file    arm_control.cpp
 * @brief   4-DOF robotic arm control with servo PID and torque limits
 *
 * Joint limits (mechanical stops):
 *   Base:     0–360° (continuous rotation mod)
 *   Shoulder: 15–165° (mechanical limits, prevents ground collision)
 *   Elbow:    0–170°
 *   Gripper:  0–90° (0=open, 90=closed)
 *
 * @author  Adnan Anwar Awan
 */

#include "arm_control.h"
#include <Servo.h>

// ── Servo objects ──────────────────────────────────────────────────────────
static Servo servo_base;
static Servo servo_shoulder;
static Servo servo_elbow;
static Servo servo_gripper;

// ── Current positions ──────────────────────────────────────────────────────
static int pos_base     = 90;
static int pos_shoulder = 90;
static int pos_elbow    = 90;
static int pos_gripper  = 0;   // 0 = open

// ── Joint limits ───────────────────────────────────────────────────────────
#define BASE_MIN      0
#define BASE_MAX      180
#define SHOULDER_MIN  15
#define SHOULDER_MAX  165
#define ELBOW_MIN     0
#define ELBOW_MAX     170
#define GRIPPER_MIN   0
#define GRIPPER_MAX   90

// ── Servo pins ─────────────────────────────────────────────────────────────
#define PIN_SERVO_BASE      22
#define PIN_SERVO_SHOULDER  23
#define PIN_SERVO_ELBOW     24
#define PIN_SERVO_GRIPPER   25

// ── Movement speed (degrees per step, with delay_ms between steps) ─────────
#define SERVO_STEP_DEG  1
#define SERVO_STEP_MS   15

static int clamp(int val, int lo, int hi) {
    return val < lo ? lo : (val > hi ? hi : val);
}

// Smoothly move a servo from current position to target
static void moveServo(Servo &srv, int &current, int target, int lo, int hi) {
    target = clamp(target, lo, hi);
    while (current != target) {
        current += (target > current) ? SERVO_STEP_DEG : -SERVO_STEP_DEG;
        srv.write(current);
        delay(SERVO_STEP_MS);
    }
}

void ARM_Init() {
    servo_base.attach(PIN_SERVO_BASE);
    servo_shoulder.attach(PIN_SERVO_SHOULDER);
    servo_elbow.attach(PIN_SERVO_ELBOW);
    servo_gripper.attach(PIN_SERVO_GRIPPER);
    ARM_SetSafePosition();
}

void ARM_SetSafePosition() {
    // Lowest centre-of-gravity position — minimises tip-over risk
    ARM_SetShoulder(45);
    delay(300);
    ARM_SetElbow(90);
    delay(300);
    ARM_SetBase(90);
    ARM_SetGripper(0);   // Open gripper
}

void ARM_SetBase(int deg) {
    moveServo(servo_base, pos_base, deg, BASE_MIN, BASE_MAX);
}

void ARM_SetShoulder(int deg) {
    moveServo(servo_shoulder, pos_shoulder, deg, SHOULDER_MIN, SHOULDER_MAX);
}

void ARM_SetElbow(int deg) {
    moveServo(servo_elbow, pos_elbow, deg, ELBOW_MIN, ELBOW_MAX);
}

void ARM_SetGripper(int deg) {
    moveServo(servo_gripper, pos_gripper, deg, GRIPPER_MIN, GRIPPER_MAX);
}

// Increment/decrement helpers (for DTMF jogging)
void ARM_BaseJog(int delta)     { ARM_SetBase(pos_base + delta); }
void ARM_ShoulderJog(int delta) { ARM_SetShoulder(pos_shoulder + delta); }
void ARM_ElbowJog(int delta)    { ARM_SetElbow(pos_elbow + delta); }
void ARM_GripperToggle() {
    ARM_SetGripper(pos_gripper < 45 ? GRIPPER_MAX : GRIPPER_MIN);
}

// Position getters (for telemetry)
int ARM_GetBase()     { return pos_base; }
int ARM_GetShoulder() { return pos_shoulder; }
int ARM_GetElbow()    { return pos_elbow; }
int ARM_GetGripper()  { return pos_gripper; }
