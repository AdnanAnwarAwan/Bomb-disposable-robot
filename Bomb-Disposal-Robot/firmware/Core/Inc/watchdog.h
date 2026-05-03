#pragma once
#include <stdbool.h>
void WATCHDOG_Init();
void WATCHDOG_Kick();
bool WATCHDOG_IsExpired();
