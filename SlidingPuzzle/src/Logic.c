#include "Logic.h"

#include <stdbool.h>  // bool, false, true
#include <stddef.h>   // NULL, size_t
#include <stdio.h>    // snprintf
#include <stdlib.h>   // srand, rand
#include <time.h>     // time

#include "zge/core.h"
#include "zge/memory.h"

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

  g_world.pieceCount = (size_t)(g_world.size.x * g_world.size.y);

  ZGE_FreeIfAllocated(g_world.pieces);
  g_world.pieces = ZGE_Allocate(g_world.pieceCount * sizeof(struct Piece));

  SetPieceStates(PieceState_Setup);
  SetupPieceValues();
  ShufflePieces();
  FindBlankPiece();
  MakeResolvable();
  SetupPiecePositions();

  g_world.statusBar.state = StatusBarState_Show;

  g_world.singleMoves = 0;
  g_world.moves = 0;
}

void L_Update()
{
  static float statusbarTime;

  if (g_world.size.x != g_config.size.x || g_world.size.y != g_config.size.y)
  {
    I_Restart();
    L_Restart();
    A_Restart();
    G_Restart();

    statusbarTime = 0.0f;
    return;
  }

  if (I_ResetKey())
  {
    I_Restart();
    L_Restart();
    A_Restart();
    G_Restart();

    statusbarTime = 0.0f;
    return;
  }

  bool const controlKey = I_ControlKey();
  int controlX = 0;
  int controlY = 0;
  bool const controlButton = I_ControlButton(&controlX, &controlY);

  switch (g_world.state)
  {
    case WorldState_Setup:
      statusbarTime += UPDATE_PERIOD_MS;

      if (controlKey || controlButton)
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

      if (statusbarTime >= 1000.0f / STATUSBAR_FREQUENCY_HZ)
      {
        if (++g_world.statusBar.state >= StatusBarState_Count)
        {
          g_world.statusBar.state = 0;
        }

        statusbarTime = 0.0f;
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
          if (controlButton)
          {
            int const windowWidth = ZGE_GetWindowWidth();
            int const windowHeight = ZGE_GetWindowHeight();

            float const logicalWidth = (float) g_world.size.x * PIECE_SIZE + MARGIN;
            float const logicalHeight =
                (float) g_world.size.y * PIECE_SIZE + MARGIN + STATUSBAR_SIZE;

            float const logicalX =
                logicalWidth / (float) windowWidth * (float) controlX - MARGIN / 2.0f;
            float const logicalY =
                logicalHeight / (float) windowHeight * (float) controlY - MARGIN / 2.0f;

            if (logicalX >= 0.0f && logicalX < (float) g_world.size.x * PIECE_SIZE &&
                logicalY >= 0.0f && logicalY < (float) g_world.size.y * PIECE_SIZE)
            {
              size_t const x = (size_t) logicalX;
              size_t const y = (size_t) logicalY;

              size_t const blankX = g_world.blank % g_world.size.x;
              size_t const blankY = g_world.blank / g_world.size.x;

              if (x == blankX)
              {
                if (y < blankY)
                {
                  A_PlaySound(Sound_Move);
                  MovePiecesDown(blankY - y);
                }
                else if (y > blankY)
                {
                  A_PlaySound(Sound_Move);
                  MovePiecesUp(y - blankY);
                }

                g_world.state = WorldState_Moving;
              }
              else if (y == blankY)
              {
                if (x < blankX)
                {
                  A_PlaySound(Sound_Move);
                  MovePiecesRight(blankX - x);
                }
                else if (x > blankX)
                {
                  A_PlaySound(Sound_Move);
                  MovePiecesLeft(x - blankX);
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
  ZGE_Free(g_world.pieces);
}

void UpdateWindowTitle()
{
  char title[30];
  snprintf(title, sizeof(title), "Sliding Puzzle %ldx%ld", g_config.size.y, g_config.size.x);

  ZGE_SetWindowTitle(title);
}

void MovePieceLeft()
{
  size_t const index = g_world.blank + 1;

  g_world.pieces[index].posW.x -= 1.0f;

  SwapPieces(index, g_world.blank);

  g_world.blank = index;

  ++g_world.singleMoves;
  ++g_world.moves;
}

void MovePieceRight()
{
  size_t const index = g_world.blank - 1;

  g_world.pieces[index].posW.x += 1.0f;

  SwapPieces(index, g_world.blank);

  g_world.blank = index;

  ++g_world.singleMoves;
  ++g_world.moves;
}

void MovePieceUp()
{
  size_t const index = g_world.blank + g_world.size.x;

  g_world.pieces[index].posW.y += 1.0f;

  SwapPieces(index, g_world.blank);

  g_world.blank = index;

  ++g_world.singleMoves;
  ++g_world.moves;
}

void MovePieceDown()
{
  size_t const index = g_world.blank - g_world.size.x;

  g_world.pieces[index].posW.y -= 1.0f;

  SwapPieces(index, g_world.blank);

  g_world.blank = index;

  ++g_world.singleMoves;
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

  for (size_t i = 0; i < g_world.pieceCount; ++i)
  {
    g_world.pieces[i].state = PieceState_Idle;

    if (g_world.pieces[i].posW.x > g_world.pieces[i].pos.x)
    {
      g_world.pieces[i].pos.x += MOVE_STEP;

      if (g_world.pieces[i].pos.x > g_world.pieces[i].posW.x)
      {
        g_world.pieces[i].pos.x = g_world.pieces[i].posW.x;
      }

      g_world.pieces[i].state = PieceState_Moving;
      moving = true;
    }
    else if (g_world.pieces[i].posW.x < g_world.pieces[i].pos.x)
    {
      g_world.pieces[i].pos.x -= MOVE_STEP;

      if (g_world.pieces[i].pos.x < g_world.pieces[i].posW.x)
      {
        g_world.pieces[i].pos.x = g_world.pieces[i].posW.x;
      }

      g_world.pieces[i].state = PieceState_Moving;
      moving = true;
    }

    if (g_world.pieces[i].posW.y > g_world.pieces[i].pos.y)
    {
      g_world.pieces[i].pos.y += MOVE_STEP;

      if (g_world.pieces[i].pos.y > g_world.pieces[i].posW.y)
      {
        g_world.pieces[i].pos.y = g_world.pieces[i].posW.y;
      }

      g_world.pieces[i].state = PieceState_Moving;
      moving = true;
    }
    else if (g_world.pieces[i].posW.y < g_world.pieces[i].pos.y)
    {
      g_world.pieces[i].pos.y -= MOVE_STEP;

      if (g_world.pieces[i].pos.y < g_world.pieces[i].posW.y)
      {
        g_world.pieces[i].pos.y = g_world.pieces[i].posW.y;
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
  for (size_t i = 0; i < g_world.pieceCount - 1; ++i)
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
  for (size_t i = 0; i < g_world.pieceCount; ++i)
  {
    g_world.pieces[i].value = value++;
  }

  g_world.pieces[g_world.pieceCount - 1].value = 0;
}

void ShufflePieces()
{
  for (size_t i = 0; i < g_world.pieceCount; ++i)
  {
    SwapPieces(i, (size_t) rand() % g_world.pieceCount);
  }
}

void FindBlankPiece()
{
  for (size_t i = 0; i < g_world.pieceCount; ++i)
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
      g_world.pieces[i].posW.x = g_world.pieces[i].pos.x;
      g_world.pieces[i].posW.y = g_world.pieces[i].pos.y;
      ++i;
    }
  }
}

void SetPieceStates(enum PieceState const state)
{
  for (size_t i = 0; i < g_world.pieceCount; ++i)
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

    i = (i + 1) % (g_world.pieceCount - 1);
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

  for (size_t i = 0; i < g_world.pieceCount - 1; ++i)
  {
    for (size_t j = i + 1; j < g_world.pieceCount; ++j)
    {
      if (g_world.pieces[i].value > g_world.pieces[j].value && g_world.pieces[j].value != 0)
      {
        ++invs;
      }
    }
  }

  return invs;
}
