// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GAME_STATE_H
#define GAME_STATE_H

#define MIN_DICE_COUNT 2
#define MAX_DICE_COUNT 5
#define DEFAULT_DICE_COUNT 3

#define DEFAULT_MAX_SHUFFLE_TIME_MS 1000

enum State
{
  State_Shuffle,
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

  int max_shuffle_time;

  int successful_attempts;
  int failed_attempts;

  int score;

  enum State state;
};

extern struct GameState g_game_state;

#endif  // GAME_STATE_H
