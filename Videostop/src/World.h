// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef WORLD_H
#define WORLD_H

#include "Config.h"

struct Dice
{
  int value;

  enum DiceState
  {
    DiceState_Setup,
    DiceState_Idle,
    DiceState_Success,
    DiceState_Fail
  } state;
};

struct StatusBar
{
  enum StatusBarState
  {
    StatusBarState_Show = 0,
    StatusBarState_Hide,

    StatusBarState_Count
  } state;
};

struct World
{
  enum WorldState
  {
    WorldState_Setup,
    WorldState_Idle,
    WorldState_Success,
    WorldState_Fail
  } state;

  struct Dice dices[MAX_DICE_COUNT];
  int dice_count;

  struct StatusBar statusBar;

  int successful_attempts;
  int failed_attempts;
  int score;
};

extern struct World g_world;

#endif  // WORLD_H
