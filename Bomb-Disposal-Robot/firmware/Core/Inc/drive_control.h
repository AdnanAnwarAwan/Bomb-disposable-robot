#pragma once
#include <stdint.h>
void DRIVE_Init();
void DRIVE_Forward(uint32_t duration_ms);
void DRIVE_Backward(uint32_t duration_ms);
void DRIVE_Left(uint32_t duration_ms);
void DRIVE_Right(uint32_t duration_ms);
void DRIVE_Stop();
