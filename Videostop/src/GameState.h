// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "GameConfig.h"

enum State
{
  State_Setup,
  State_Idle,
  State_Success,
  State_Fail
};

struct Dice
{
  int value;
  enum State state;
};

struct GameState
{
  struct Dice dices[MAX_DICE_COUNT];
  int dice_count;

  enum State state;

  int successful_attempts;
  int failed_attempts;

  int score;
};

extern struct GameState g_game_state;

#endif  // GAME_STATE_H
