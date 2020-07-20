#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>  // bool

void I_Start();
void I_Restart();
void I_Update();
void I_Stop();

bool I_ResetKey();
bool I_ControlKey();
bool I_ControlButton();
bool I_SizeUpKey();
bool I_SizeDownKey();
bool I_SpeedUpKey();
bool I_SpeedDownKey();

#endif  // INPUT_H
