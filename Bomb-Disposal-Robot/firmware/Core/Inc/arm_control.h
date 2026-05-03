#pragma once
void ARM_Init();
void ARM_SetSafePosition();
void ARM_SetBase(int deg);
void ARM_SetShoulder(int deg);
void ARM_SetElbow(int deg);
void ARM_SetGripper(int deg);
void ARM_BaseJog(int delta);
void ARM_ShoulderJog(int delta);
void ARM_ElbowJog(int delta);
void ARM_GripperToggle();
int  ARM_GetBase();
int  ARM_GetShoulder();
int  ARM_GetElbow();
int  ARM_GetGripper();
