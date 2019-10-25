// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stddef.h>  // size_t

enum Value
{
  Value_Empty,
  Value_Food,
  Value_Wall,
  Value_Snake
};

struct Field
{
  struct
  {
    int x;
    int y;
  } pos;

  enum Value val;
};

enum Heading
{
  Heading_Left,
  Heading_Right,
  Heading_Down,
  Heading_Up
};

enum State
{
  State_Play,
  State_Success,
  State_Fail,
  State_Pause
};

struct GameState
{
  struct
  {
    int x;
    int y;
  } size;

  struct Field* fields;
  size_t field_count;

  struct Field** snake;
  size_t head;
  size_t tail;

  enum Heading heading;

  enum State state;

  double max_move_time;

  int score;
};

extern struct GameState g_game_state;

#endif  // GAME_STATE_H
