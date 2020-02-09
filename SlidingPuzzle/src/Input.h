// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>  // bool

void I_Start();
void I_Restart();
void I_Update();
void I_Stop();

bool I_ResetKey();
bool I_ControlKey();
bool I_ControlButton(int* x, int* y);

enum DirectionKeys
{
  KEY_LEFT,
  KEY_RIGHT,
  KEY_DOWN,
  KEY_UP
};

int I_PopDirectionKey();

#endif  // INPUT_H
