/**
 * @file    watchdog.cpp
 * @brief   Software dead-man switch — safe mode if heartbeat lost for 2s
 * @author  Adnan Anwar Awan
 */

#include "watchdog.h"
#include <Arduino.h>

#define WATCHDOG_TIMEOUT_MS  2000

static uint32_t last_kick_ms = 0;
static bool     initialized   = false;

void WATCHDOG_Init() {
    last_kick_ms = millis();
    initialized  = true;
}

void WATCHDOG_Kick() {
    last_kick_ms = millis();
}

bool WATCHDOG_IsExpired() {
    if (!initialized) return false;
    return (millis() - last_kick_ms) > WATCHDOG_TIMEOUT_MS;
}
