#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>  // bool

void I_Start();
void I_Restart();
void I_Update();
void I_Stop();

bool I_ResetKey();
bool I_PauseKey();

enum Key
{
  KEY_LEFT,
  KEY_RIGHT,
  KEY_DOWN,
  KEY_UP
};

int I_GetDirectionKey();
int I_PopDirectionKey();

#endif  // INPUT_H
