// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Logic.h"

#include <stdbool.h>  // bool, false, true
#include <stddef.h>   // size_t
#include <stdio.h>    // snprintf
#include <stdlib.h>   // free, malloc, srand, rand
#include <time.h>     // time

#include <GL/freeglut.h>

#include "GameConfig.h"
#include "GameState.h"
#include "Graphics.h"
#include "Input.h"
#include "Sound.h"

static void UpdateWindowTitle();

static void MovePieceLeft();
static void MovePieceRight();
static void MovePieceUp();
static void MovePieceDown();
static void MovePiecesLeft(size_t count);
static void MovePiecesRight(size_t count);
static void MovePiecesUp(size_t count);
static void MovePiecesDown(size_t count);
static bool MovePieces();
static bool IsResolved();

static void SetupPieceValues();
static void ShufflePieces();
static void FindBlankPiece();
static void SetupPiecePositions();
static void SetPieceStates(enum State state);
static void MakeResolvable();

static void SwapPieces(size_t index1, size_t index2);
static bool IsResolvable();
static int CountInversions();

void L_Start()
{
  UpdateWindowTitle();

  srand((unsigned int) time(NULL));

  L_Restart();
}

void L_Restart()
{
  g_game_state.size.x = g_game_config.size.x;
  g_game_state.size.y = g_game_config.size.y;

  g_game_state.piece_count = (size_t)(g_game_state.size.x * g_game_state.size.y);

  if (g_game_state.pieces != NULL)
  {
    free(g_game_state.pieces);
  }

  g_game_state.pieces = malloc(g_game_state.piece_count * sizeof(struct Piece));

  SetupPieceValues();
  ShufflePieces();
  FindBlankPiece();
  MakeResolvable();
  SetupPiecePositions();
  SetPieceStates(State_Setup);

  g_game_state.state = State_Setup;

  g_game_state.single_moves = 0;
  g_game_state.moves = 0;
}

void L_Update()
{
  if (g_game_state.size.x != g_game_config.size.x || g_game_state.size.y != g_game_config.size.y)
  {
    I_Restart();
    L_Restart();
    S_Restart();
    G_Restart();
    return;
  }

  if (I_ResetKey())
  {
    I_Restart();
    L_Restart();
    S_Restart();
    // G_Restart();
    return;
  }

  bool control_key = I_ControlKey();
  int control_x = 0;
  int control_y = 0;
  bool control_button = I_ControlButton(&control_x, &control_y);

  switch (g_game_state.state)
  {
    case State_Setup:
      if (control_key || control_button)
      {
        S_PlaySound(Sound_Start);
        SetPieceStates(State_Idle);

        g_game_state.state = State_Idle;
      }
      break;

    case State_Idle:
      switch (I_PopDirectionKey())
      {
        case GLUT_KEY_LEFT:
          if (g_game_state.blank % g_game_state.size.x != g_game_state.size.x - 1)
          {
            S_PlaySound(Sound_Move);
            MovePieceLeft();

            g_game_state.state = State_Moving;
          }
          else
          {
            S_PlaySound(Sound_CannotMove);
          }
          break;

        case GLUT_KEY_RIGHT:
          if (g_game_state.blank % g_game_state.size.x != 0)
          {
            S_PlaySound(Sound_Move);
            MovePieceRight();

            g_game_state.state = State_Moving;
          }
          else
          {
            S_PlaySound(Sound_CannotMove);
          }
          break;

        case GLUT_KEY_DOWN:
          if (g_game_state.blank >= g_game_state.size.x)
          {
            S_PlaySound(Sound_Move);
            MovePieceDown();

            g_game_state.state = State_Moving;
          }
          else
          {
            S_PlaySound(Sound_CannotMove);
          }
          break;

        case GLUT_KEY_UP:
          if (g_game_state.blank + g_game_state.size.x < g_game_state.size.x * g_game_state.size.y)
          {
            S_PlaySound(Sound_Move);
            MovePieceUp();

            g_game_state.state = State_Moving;
          }
          else
          {
            S_PlaySound(Sound_CannotMove);
          }
          break;

        default:
          if (control_button)
          {
            size_t x = (size_t)((float) control_x / (float) glutGet(GLUT_WINDOW_WIDTH) *
                                (float) g_game_state.size.x);
            size_t y = (size_t)((float) control_y / (float) glutGet(GLUT_WINDOW_HEIGHT) *
                                (float) g_game_state.size.y);

            size_t blank_x = g_game_state.blank % g_game_state.size.x;
            size_t blank_y = g_game_state.blank / g_game_state.size.x;

            if (x == blank_x)
            {
              if (y < blank_y)
              {
                S_PlaySound(Sound_Move);
                MovePiecesDown(blank_y - y);
              }
              else if (y > blank_y)
              {
                S_PlaySound(Sound_Move);
                MovePiecesUp(y - blank_y);
              }
            }
            else if (y == blank_y)
            {
              if (x < blank_x)
              {
                S_PlaySound(Sound_Move);
                MovePiecesRight(blank_x - x);
              }
              else if (x > blank_x)
              {
                S_PlaySound(Sound_Move);
                MovePiecesLeft(x - blank_x);
              }
            }
            else
            {
              S_PlaySound(Sound_CannotMove);
            }

            g_game_state.state = State_Moving;
          }
      }
      break;

    case State_Moving:
      if (!MovePieces())
      {
        if (IsResolved())
        {
          S_PlaySound(Sound_Success);
          SetPieceStates(State_Success);

          g_game_state.state = State_Success;
        }
        else
        {
          g_game_state.state = State_Idle;
        }
      }
      break;

    case State_Success:
      break;
  }
}

void L_Stop()
{
  free(g_game_state.pieces);
}

void UpdateWindowTitle()
{
  char buf[30];
  snprintf(buf, sizeof(buf), "Sliding Puzzle %ldx%ld", g_game_config.size.y, g_game_config.size.x);

  glutSetWindowTitle(buf);
}

void MovePieceLeft()
{
  size_t index = g_game_state.blank + 1;

  g_game_state.pieces[index].pos_w.x -= 1.0f;

  SwapPieces(index, g_game_state.blank);

  g_game_state.blank = index;

  ++g_game_state.single_moves;
  ++g_game_state.moves;
}

void MovePieceRight()
{
  size_t index = g_game_state.blank - 1;

  g_game_state.pieces[index].pos_w.x += 1.0f;

  SwapPieces(index, g_game_state.blank);

  g_game_state.blank = index;

  ++g_game_state.single_moves;
  ++g_game_state.moves;
}

void MovePieceUp()
{
  size_t index = g_game_state.blank + g_game_state.size.x;

  g_game_state.pieces[index].pos_w.y += 1.0f;

  SwapPieces(index, g_game_state.blank);

  g_game_state.blank = index;

  ++g_game_state.single_moves;
  ++g_game_state.moves;
}

void MovePieceDown()
{
  size_t index = g_game_state.blank - g_game_state.size.x;

  g_game_state.pieces[index].pos_w.y -= 1.0f;

  SwapPieces(index, g_game_state.blank);

  g_game_state.blank = index;

  ++g_game_state.single_moves;
  ++g_game_state.moves;
}

void MovePiecesLeft(size_t count)
{
  int temp = g_game_state.moves;

  for (size_t i = 0; i < count; ++i)
  {
    MovePieceLeft();
  }

  g_game_state.moves = temp;
  ++g_game_state.moves;
}

void MovePiecesRight(size_t count)
{
  int temp = g_game_state.moves;

  for (size_t i = 0; i < count; ++i)
  {
    MovePieceRight();
  }

  g_game_state.moves = temp;
  ++g_game_state.moves;
}

void MovePiecesUp(size_t count)
{
  int temp = g_game_state.moves;

  for (size_t i = 0; i < count; ++i)
  {
    MovePieceUp();
  }

  g_game_state.moves = temp;
  ++g_game_state.moves;
}

void MovePiecesDown(size_t count)
{
  int temp = g_game_state.moves;

  for (size_t i = 0; i < count; ++i)
  {
    MovePieceDown();
  }

  g_game_state.moves = temp;
  ++g_game_state.moves;
}

bool MovePieces()
{
  bool moving = false;

  for (size_t i = 0; i < g_game_state.piece_count; ++i)
  {
    g_game_state.pieces[i].state = State_Idle;

    if (g_game_state.pieces[i].pos_w.x > g_game_state.pieces[i].pos.x)
    {
      g_game_state.pieces[i].pos.x += MOVE_STEP;

      if (g_game_state.pieces[i].pos.x > g_game_state.pieces[i].pos_w.x)
      {
        g_game_state.pieces[i].pos.x = g_game_state.pieces[i].pos_w.x;
      }

      g_game_state.pieces[i].state = State_Moving;
      moving = true;
    }
    else if (g_game_state.pieces[i].pos_w.x < g_game_state.pieces[i].pos.x)
    {
      g_game_state.pieces[i].pos.x -= MOVE_STEP;

      if (g_game_state.pieces[i].pos.x < g_game_state.pieces[i].pos_w.x)
      {
        g_game_state.pieces[i].pos.x = g_game_state.pieces[i].pos_w.x;
      }

      g_game_state.pieces[i].state = State_Moving;
      moving = true;
    }

    if (g_game_state.pieces[i].pos_w.y > g_game_state.pieces[i].pos.y)
    {
      g_game_state.pieces[i].pos.y += MOVE_STEP;

      if (g_game_state.pieces[i].pos.y > g_game_state.pieces[i].pos_w.y)
      {
        g_game_state.pieces[i].pos.y = g_game_state.pieces[i].pos_w.y;
      }

      g_game_state.pieces[i].state = State_Moving;
      moving = true;
    }
    else if (g_game_state.pieces[i].pos_w.y < g_game_state.pieces[i].pos.y)
    {
      g_game_state.pieces[i].pos.y -= MOVE_STEP;

      if (g_game_state.pieces[i].pos.y < g_game_state.pieces[i].pos_w.y)
      {
        g_game_state.pieces[i].pos.y = g_game_state.pieces[i].pos_w.y;
      }

      g_game_state.pieces[i].state = State_Moving;
      moving = true;
    }
  }

  return moving;
}

bool IsResolved()
{
  int value = 1;
  for (size_t i = 0; i < g_game_state.piece_count - 1; ++i)
  {
    if (g_game_state.pieces[i].value != value)
    {
      return false;
    }

    ++value;
  }

  return true;
}

void SetupPieceValues()
{
  int value = 1;
  for (size_t i = 0; i < g_game_state.piece_count; ++i)
  {
    g_game_state.pieces[i].value = value++;
  }

  g_game_state.pieces[g_game_state.piece_count - 1].value = 0;
}

void ShufflePieces()
{
  for (size_t i = 0; i < g_game_state.piece_count; ++i)
  {
    SwapPieces(i, (size_t) rand() % g_game_state.piece_count);
  }
}

void FindBlankPiece()
{
  for (size_t i = 0; i < g_game_state.piece_count; ++i)
  {
    if (g_game_state.pieces[i].value == 0)
    {
      g_game_state.blank = i;
      break;
    }
  }
}

void SetupPiecePositions()
{
  size_t i = 0;
  for (size_t y = 0; y < g_game_state.size.y; ++y)
  {
    for (size_t x = 0; x < g_game_state.size.x; ++x)
    {
      g_game_state.pieces[i].pos.x = (float) x;
      g_game_state.pieces[i].pos.y = (float) (g_game_state.size.y - y - 1);
      g_game_state.pieces[i].pos_w.x = g_game_state.pieces[i].pos.x;
      g_game_state.pieces[i].pos_w.y = g_game_state.pieces[i].pos.y;
      ++i;
    }
  }
}

void SetPieceStates(enum State state)
{
  for (size_t i = 0; i < g_game_state.piece_count; ++i)
  {
    g_game_state.pieces[i].state = state;
  }
}

void MakeResolvable()
{
  size_t i = 0;
  while (!IsResolvable())
  {
    if (g_game_state.pieces[i].value > g_game_state.pieces[i + 1].value &&
        g_game_state.pieces[i + 1].value != 0)
    {
      SwapPieces(i, i + 1);
    }
    else if (g_game_state.pieces[i].value == 0)
    {
      SwapPieces(i, i + 1);
      g_game_state.blank = i + 1;
    }

    i = (i + 1) % (g_game_state.piece_count - 1);
  }
}

void SwapPieces(size_t index1, size_t index2)
{
  struct Piece temp;
  temp = g_game_state.pieces[index1];
  g_game_state.pieces[index1] = g_game_state.pieces[index2];
  g_game_state.pieces[index2] = temp;
}

bool IsResolvable()
{
  if (g_game_state.size.x % 2 == 1)
  {
    return CountInversions() % 2 == 0;
  }

  if ((g_game_state.size.y - (g_game_state.blank / g_game_state.size.x)) % 2 == 1)
  {
    return CountInversions() % 2 == 0;
  }

  return CountInversions() % 2 == 1;
}

int CountInversions()
{
  int invs = 0;

  for (size_t i = 0; i < g_game_state.piece_count - 1; ++i)
  {
    for (size_t j = i + 1; j < g_game_state.piece_count; ++j)
    {
      if (g_game_state.pieces[i].value > g_game_state.pieces[j].value &&
          g_game_state.pieces[j].value != 0)
      {
        ++invs;
      }
    }
  }

  return invs;
}
