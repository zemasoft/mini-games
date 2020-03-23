// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef WORLD_H
#define WORLD_H

#include <stddef.h>  // size_t

#include "Config.h"

struct Piece
{
  enum PieceState
  {
    PieceState_Setup,
    PieceState_Idle,
    PieceState_Moving,
    PieceState_Success
  } state;

  int value;

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

  struct
  {
    char value[STRING_SIZE];
    float width;
    float height;
  } string;
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
    WorldState_Moving,
    WorldState_Success
  } state;

  struct
  {
    size_t x;
    size_t y;
  } size;

  struct Piece* pieces;
  size_t piece_count;
  size_t blank;

  struct StatusBar statusBar;

  int single_moves;
  int moves;
};

extern struct World g_world;

#endif  // WORLD_H
