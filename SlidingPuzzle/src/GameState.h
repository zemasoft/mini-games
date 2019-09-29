// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdbool.h>  // bool
#include <stddef.h>   // size_t

#define MIN_SIZE 2
#define MAX_SIZE 10
#define DEFAULT_SIZE 3

enum State
{
  State_Idle = 0,
  State_Moving,
  State_Success
};

struct Piece
{
  struct
  {
    float x;
    float y;
  } pos;

  struct
  {
    float x;
    float y;
  } pos_w;

  int value;

  enum State state;

  char string[5];
  float string_width;
  float string_height;
};

struct GameState
{
  size_t y_size;
  size_t x_size;

  struct Piece* pieces;
  size_t piece_count;
  size_t blank;

  enum State state;

  bool left_key;
  bool right_key;
  bool up_key;
  bool down_key;
  bool reset_key;
  bool mouse_button;
  int mouse_x;
  int mouse_y;

  int single_moves;
  int moves;
};

extern struct GameState g_game_state;

#endif  // GAME_STATE_H
