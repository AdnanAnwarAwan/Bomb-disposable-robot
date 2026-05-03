/**
 * @file    dtmf_decoder.cpp
 * @brief   MT8870 DTMF to command decoder
 *          Uses command-pulse strategy (500ms burst per keypress)
 *          to prevent over-steering at high GSM latency
 *
 * DTMF binary table (Q4:Q3:Q2:Q1):
 *   Key 2 = 0010 = 2  → Forward
 *   Key 4 = 0100 = 4  → Left
 *   Key 5 = 0101 = 5  → Stop
 *   Key 6 = 0110 = 6  → Right
 *   Key 8 = 1000 = 8  → Backward
 *   Key 1 = 0001 = 1  → Arm Base CW
 *   Key 3 = 0011 = 3  → Arm Base CCW
 *   Key 7 = 0111 = 7  → Shoulder Up
 *   Key 9 = 1001 = 9  → Shoulder Down
 *   Key 0 = 1010 = 10 → Gripper Open
 *   Key * = 1011 = 11 → Gripper Close
 *   Key # = 1100 = 12 → Emergency Stop
 *
 * @author  Adnan Anwar Awan
 */

#include "dtmf_decoder.h"
#include "drive_control.h"
#include "arm_control.h"
#include <Arduino.h>

#define PULSE_MS        500     // Each keypress triggers 500ms movement
#define ARM_JOG_DEG     15      // Degrees per DTMF jog command

void DTMF_ProcessCommand(uint8_t code) {
    Serial.print(F("DTMF: "));
    Serial.println(code);

    switch (code) {
        // ── Drive commands ─────────────────────────────────────────────
        case 2:  DRIVE_Forward(PULSE_MS);   break;
        case 4:  DRIVE_Left(PULSE_MS);      break;
        case 5:  DRIVE_Stop();              break;
        case 6:  DRIVE_Right(PULSE_MS);     break;
        case 8:  DRIVE_Backward(PULSE_MS);  break;

        // ── Arm commands ───────────────────────────────────────────────
        case 1:  ARM_BaseJog(+ARM_JOG_DEG);      break;  // Base CW
        case 3:  ARM_BaseJog(-ARM_JOG_DEG);      break;  // Base CCW
        case 7:  ARM_ShoulderJog(+ARM_JOG_DEG);  break;  // Shoulder up
        case 9:  ARM_ShoulderJog(-ARM_JOG_DEG);  break;  // Shoulder down
        case 10: ARM_SetGripper(0);               break;  // Open
        case 11: ARM_SetGripper(90);              break;  // Close

        // ── Emergency stop ─────────────────────────────────────────────
        case 12:
            DRIVE_Stop();
            ARM_SetSafePosition();
            Serial.println(F("ESTOP via DTMF #"));
            break;

        default: break;
    }
}
