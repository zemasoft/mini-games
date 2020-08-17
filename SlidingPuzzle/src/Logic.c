#include "Logic.h"

#include <stdbool.h>  // bool, false, true
#include <stddef.h>   // size_t
#include <stdio.h>    // snprintf
#include <stdlib.h>   // free, malloc, srand, rand
#include <time.h>     // time

#include "zge/zge.h"

#include "Audio.h"
#include "Config.h"
#include "Graphics.h"
#include "Input.h"
#include "World.h"

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

static void SetPieceStates(enum PieceState state);
static void SetupPieceValues();
static void ShufflePieces();
static void FindBlankPiece();
static void SetupPiecePositions();
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
  g_world.state = WorldState_Setup;

  g_world.size.x = g_config.size.x;
  g_world.size.y = g_config.size.y;

  g_world.piece_count = (size_t)(g_world.size.x * g_world.size.y);

  if (g_world.pieces != NULL)
  {
    free(g_world.pieces);
  }

  g_world.pieces = malloc(g_world.piece_count * sizeof(struct Piece));

  SetPieceStates(PieceState_Setup);
  SetupPieceValues();
  ShufflePieces();
  FindBlankPiece();
  MakeResolvable();
  SetupPiecePositions();

  g_world.statusBar.state = StatusBarState_Show;

  g_world.single_moves = 0;
  g_world.moves = 0;
}

void L_Update()
{
  static float statusbar_time;

  if (g_world.size.x != g_config.size.x || g_world.size.y != g_config.size.y)
  {
    I_Restart();
    L_Restart();
    A_Restart();
    G_Restart();

    statusbar_time = 0.0f;
    return;
  }

  if (I_ResetKey())
  {
    I_Restart();
    L_Restart();
    A_Restart();
    G_Restart();

    statusbar_time = 0.0f;
    return;
  }

  bool const control_key = I_ControlKey();
  int control_x = 0;
  int control_y = 0;
  bool const control_button = I_ControlButton(&control_x, &control_y);

  switch (g_world.state)
  {
    case WorldState_Setup:
      statusbar_time += UPDATE_PERIOD_MS;

      if (control_key || control_button)
      {
        A_PlaySound(Sound_Start);
        SetPieceStates(PieceState_Idle);

        g_world.state = WorldState_Idle;
        break;
      }

      switch (I_PopDirectionKey())
      {
        case KEY_LEFT:
          if (g_config.size.x > MIN_SIZE)
          {
            --g_config.size.x;

            UpdateWindowTitle();
          }
          break;

        case KEY_RIGHT:
          if (g_config.size.x < MAX_SIZE)
          {
            ++g_config.size.x;

            UpdateWindowTitle();
          }
          break;

        case KEY_DOWN:
          if (g_config.size.y > MIN_SIZE)
          {
            --g_config.size.y;

            UpdateWindowTitle();
          }
          break;

        case KEY_UP:
          if (g_config.size.y < MAX_SIZE)
          {
            ++g_config.size.y;

            UpdateWindowTitle();
          }
          break;
      }

      if (statusbar_time >= 1000.0f / STATUSBAR_FREQUENCY_HZ)
      {
        if (++g_world.statusBar.state >= StatusBarState_Count)
        {
          g_world.statusBar.state = 0;
        }

        statusbar_time = 0.0f;
      }
      break;

    case WorldState_Idle:
      switch (I_PopDirectionKey())
      {
        case KEY_LEFT:
          if (g_world.blank % g_world.size.x != g_world.size.x - 1)
          {
            A_PlaySound(Sound_Move);
            MovePieceLeft();

            g_world.state = WorldState_Moving;
          }
          else
          {
            A_PlaySound(Sound_CannotMove);
          }
          break;

        case KEY_RIGHT:
          if (g_world.blank % g_world.size.x != 0)
          {
            A_PlaySound(Sound_Move);
            MovePieceRight();

            g_world.state = WorldState_Moving;
          }
          else
          {
            A_PlaySound(Sound_CannotMove);
          }
          break;

        case KEY_DOWN:
          if (g_world.blank >= g_world.size.x)
          {
            A_PlaySound(Sound_Move);
            MovePieceDown();

            g_world.state = WorldState_Moving;
          }
          else
          {
            A_PlaySound(Sound_CannotMove);
          }
          break;

        case KEY_UP:
          if (g_world.blank + g_world.size.x < g_world.size.x * g_world.size.y)
          {
            A_PlaySound(Sound_Move);
            MovePieceUp();

            g_world.state = WorldState_Moving;
          }
          else
          {
            A_PlaySound(Sound_CannotMove);
          }
          break;

        default:
          if (control_button)
          {
            int const window_width = zgeGetWindowWidth();
            int const window_height = zgeGetWindowHeight();

            float const logical_width = (float) g_world.size.x * PIECE_SIZE + MARGIN;
            float const logical_height =
                (float) g_world.size.y * PIECE_SIZE + MARGIN + STATUSBAR_SIZE;

            float const logical_x =
                logical_width / (float) window_width * (float) control_x - MARGIN / 2.0f;
            float const logical_y =
                logical_height / (float) window_height * (float) control_y - MARGIN / 2.0f;

            if (logical_x >= 0.0f && logical_x < (float) g_world.size.x * PIECE_SIZE &&
                logical_y >= 0.0f && logical_y < (float) g_world.size.y * PIECE_SIZE)
            {
              size_t const x = (size_t) logical_x;
              size_t const y = (size_t) logical_y;

              size_t const blank_x = g_world.blank % g_world.size.x;
              size_t const blank_y = g_world.blank / g_world.size.x;

              if (x == blank_x)
              {
                if (y < blank_y)
                {
                  A_PlaySound(Sound_Move);
                  MovePiecesDown(blank_y - y);
                }
                else if (y > blank_y)
                {
                  A_PlaySound(Sound_Move);
                  MovePiecesUp(y - blank_y);
                }

                g_world.state = WorldState_Moving;
              }
              else if (y == blank_y)
              {
                if (x < blank_x)
                {
                  A_PlaySound(Sound_Move);
                  MovePiecesRight(blank_x - x);
                }
                else if (x > blank_x)
                {
                  A_PlaySound(Sound_Move);
                  MovePiecesLeft(x - blank_x);
                }

                g_world.state = WorldState_Moving;
              }
              else
              {
                A_PlaySound(Sound_CannotMove);
              }
            }
          }
      }
      break;

    case WorldState_Moving:
      if (!MovePieces())
      {
        if (IsResolved())
        {
          A_PlaySound(Sound_Success);
          SetPieceStates(PieceState_Success);

          g_world.state = WorldState_Success;
        }
        else
        {
          g_world.state = WorldState_Idle;
        }
      }
      break;

    case WorldState_Success:
      break;
  }
}

void L_Stop()
{
  free(g_world.pieces);
}

void UpdateWindowTitle()
{
  char title[30];
  snprintf(title, sizeof(title), "Sliding Puzzle %ldx%ld", g_config.size.y, g_config.size.x);

  zgeSetWindowTitle(title);
}

void MovePieceLeft()
{
  size_t const index = g_world.blank + 1;

  g_world.pieces[index].pos_w.x -= 1.0f;

  SwapPieces(index, g_world.blank);

  g_world.blank = index;

  ++g_world.single_moves;
  ++g_world.moves;
}

void MovePieceRight()
{
  size_t const index = g_world.blank - 1;

  g_world.pieces[index].pos_w.x += 1.0f;

  SwapPieces(index, g_world.blank);

  g_world.blank = index;

  ++g_world.single_moves;
  ++g_world.moves;
}

void MovePieceUp()
{
  size_t const index = g_world.blank + g_world.size.x;

  g_world.pieces[index].pos_w.y += 1.0f;

  SwapPieces(index, g_world.blank);

  g_world.blank = index;

  ++g_world.single_moves;
  ++g_world.moves;
}

void MovePieceDown()
{
  size_t const index = g_world.blank - g_world.size.x;

  g_world.pieces[index].pos_w.y -= 1.0f;

  SwapPieces(index, g_world.blank);

  g_world.blank = index;

  ++g_world.single_moves;
  ++g_world.moves;
}

void MovePiecesLeft(size_t const count)
{
  int const temp = g_world.moves;

  for (size_t i = 0; i < count; ++i)
  {
    MovePieceLeft();
  }

  g_world.moves = temp;
  ++g_world.moves;
}

void MovePiecesRight(size_t const count)
{
  int const temp = g_world.moves;

  for (size_t i = 0; i < count; ++i)
  {
    MovePieceRight();
  }

  g_world.moves = temp;
  ++g_world.moves;
}

void MovePiecesUp(size_t const count)
{
  int const temp = g_world.moves;

  for (size_t i = 0; i < count; ++i)
  {
    MovePieceUp();
  }

  g_world.moves = temp;
  ++g_world.moves;
}

void MovePiecesDown(size_t const count)
{
  int const temp = g_world.moves;

  for (size_t i = 0; i < count; ++i)
  {
    MovePieceDown();
  }

  g_world.moves = temp;
  ++g_world.moves;
}

bool MovePieces()
{
  bool moving = false;

  for (size_t i = 0; i < g_world.piece_count; ++i)
  {
    g_world.pieces[i].state = PieceState_Idle;

    if (g_world.pieces[i].pos_w.x > g_world.pieces[i].pos.x)
    {
      g_world.pieces[i].pos.x += MOVE_STEP;

      if (g_world.pieces[i].pos.x > g_world.pieces[i].pos_w.x)
      {
        g_world.pieces[i].pos.x = g_world.pieces[i].pos_w.x;
      }

      g_world.pieces[i].state = PieceState_Moving;
      moving = true;
    }
    else if (g_world.pieces[i].pos_w.x < g_world.pieces[i].pos.x)
    {
      g_world.pieces[i].pos.x -= MOVE_STEP;

      if (g_world.pieces[i].pos.x < g_world.pieces[i].pos_w.x)
      {
        g_world.pieces[i].pos.x = g_world.pieces[i].pos_w.x;
      }

      g_world.pieces[i].state = PieceState_Moving;
      moving = true;
    }

    if (g_world.pieces[i].pos_w.y > g_world.pieces[i].pos.y)
    {
      g_world.pieces[i].pos.y += MOVE_STEP;

      if (g_world.pieces[i].pos.y > g_world.pieces[i].pos_w.y)
      {
        g_world.pieces[i].pos.y = g_world.pieces[i].pos_w.y;
      }

      g_world.pieces[i].state = PieceState_Moving;
      moving = true;
    }
    else if (g_world.pieces[i].pos_w.y < g_world.pieces[i].pos.y)
    {
      g_world.pieces[i].pos.y -= MOVE_STEP;

      if (g_world.pieces[i].pos.y < g_world.pieces[i].pos_w.y)
      {
        g_world.pieces[i].pos.y = g_world.pieces[i].pos_w.y;
      }

      g_world.pieces[i].state = PieceState_Moving;
      moving = true;
    }
  }

  return moving;
}

bool IsResolved()
{
  int value = 1;
  for (size_t i = 0; i < g_world.piece_count - 1; ++i)
  {
    if (g_world.pieces[i].value != value)
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
  for (size_t i = 0; i < g_world.piece_count; ++i)
  {
    g_world.pieces[i].value = value++;
  }

  g_world.pieces[g_world.piece_count - 1].value = 0;
}

void ShufflePieces()
{
  for (size_t i = 0; i < g_world.piece_count; ++i)
  {
    SwapPieces(i, (size_t) rand() % g_world.piece_count);
  }
}

void FindBlankPiece()
{
  for (size_t i = 0; i < g_world.piece_count; ++i)
  {
    if (g_world.pieces[i].value == 0)
    {
      g_world.blank = i;
      break;
    }
  }
}

void SetupPiecePositions()
{
  size_t i = 0;
  for (size_t y = 0; y < g_world.size.y; ++y)
  {
    for (size_t x = 0; x < g_world.size.x; ++x)
    {
      g_world.pieces[i].pos.x = (float) x;
      g_world.pieces[i].pos.y = (float) (g_world.size.y - y - 1);
      g_world.pieces[i].pos_w.x = g_world.pieces[i].pos.x;
      g_world.pieces[i].pos_w.y = g_world.pieces[i].pos.y;
      ++i;
    }
  }
}

void SetPieceStates(enum PieceState const state)
{
  for (size_t i = 0; i < g_world.piece_count; ++i)
  {
    g_world.pieces[i].state = state;
  }
}

void MakeResolvable()
{
  size_t i = 0;
  while (!IsResolvable())
  {
    if (g_world.pieces[i].value > g_world.pieces[i + 1].value && g_world.pieces[i + 1].value != 0)
    {
      SwapPieces(i, i + 1);
    }
    else if (g_world.pieces[i].value == 0)
    {
      SwapPieces(i, i + 1);
      g_world.blank = i + 1;
    }

    i = (i + 1) % (g_world.piece_count - 1);
  }
}

void SwapPieces(size_t const index1, size_t const index2)
{
  struct Piece temp;
  temp = g_world.pieces[index1];
  g_world.pieces[index1] = g_world.pieces[index2];
  g_world.pieces[index2] = temp;
}

bool IsResolvable()
{
  if (g_world.size.x % 2 == 1)
  {
    return CountInversions() % 2 == 0;
  }

  if ((g_world.size.y - (g_world.blank / g_world.size.x)) % 2 == 1)
  {
    return CountInversions() % 2 == 0;
  }

  return CountInversions() % 2 == 1;
}

int CountInversions()
{
  int invs = 0;

  for (size_t i = 0; i < g_world.piece_count - 1; ++i)
  {
    for (size_t j = i + 1; j < g_world.piece_count; ++j)
    {
      if (g_world.pieces[i].value > g_world.pieces[j].value && g_world.pieces[j].value != 0)
      {
        ++invs;
      }
    }
  }

  return invs;
}
