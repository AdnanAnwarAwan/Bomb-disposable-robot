#pragma once
#include <stdbool.h>
void  IMU_Init();
void  IMU_Update();
bool  IMU_TipOverDetected();
float IMU_GetRoll();
float IMU_GetPitch();
