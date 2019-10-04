// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stddef.h>  // size_t

#define MIN_SIZE 2
#define MAX_SIZE 10
#define DEFAULT_SIZE 3

enum State
{
  State_Idle,
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

  struct
  {
    char value[4];  // SIZE = floor(log10(MAX_SIZE^2)) + 1 + 1
    float width;
    float height;
  } string;
};

struct GameState
{
  struct
  {
    size_t x;
    size_t y;
  } size;

  struct Piece* pieces;
  size_t piece_count;
  size_t blank;

  enum State state;

  int single_moves;
  int moves;
};

extern struct GameState g_game_state;

#endif  // GAME_STATE_H
