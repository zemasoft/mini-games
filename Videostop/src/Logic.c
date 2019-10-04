// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Logic.h"

#include <stdbool.h>  // bool, false, true
#include <stdlib.h>   // srand, rand
#include <time.h>     // time

#include <GL/freeglut.h>

#include "GameState.h"
#include "Input.h"
#include "Sound.h"

#define MAX_STOP_TIME_MS 1000

static void Restart();
static void ShuffleDices();
static bool CountScore();

void L_Start()
{
  srand((unsigned int) time(NULL));

  Restart();
}

void L_Update()
{
  static int before;
  static int shuffle_time;
  static int stop_time;
  static bool controls_blocked;

  int now = glutGet(GLUT_ELAPSED_TIME);
  int elapsed = now - before;
  before = now;

  if (I_ResetKey())
  {
    I_Restart();
    Restart();

    shuffle_time = 0;
    stop_time = 0;
    controls_blocked = false;
    return;
  }

  bool control_key = I_ControlKey();
  bool control_button = I_ControlButton();

  switch (g_game_state.state)
  {
    case State_Shuffle:
      shuffle_time += elapsed;

      if (!controls_blocked)
      {
        if (control_key || control_button)
        {
          if (CountScore())
          {
            S_PlaySound(Sound_Success);

            g_game_state.state = State_Success;
            stop_time = 0;
            controls_blocked = true;
          }
          else
          {
            S_PlaySound(Sound_Fail);

            g_game_state.state = State_Fail;
            stop_time = 0;
            controls_blocked = true;
          }
          break;
        }
      }

      if (shuffle_time >= g_game_state.max_shuffle_time)
      {
        S_PlaySound(Sound_Shuffle);

        ShuffleDices();
        shuffle_time = 0;
        controls_blocked = false;
      }
      break;

    case State_Success:
    case State_Fail:
      stop_time += elapsed;

      if (stop_time >= MAX_STOP_TIME_MS)
      {
        g_game_state.state = State_Shuffle;
        shuffle_time = 0;
      }
      break;
  }
}

void L_Stop()
{
}

void Restart()
{
  ShuffleDices();

  g_game_state.successful_attempts = 0;
  g_game_state.failed_attempts = 0;

  g_game_state.score = 0;

  g_game_state.state = State_Shuffle;
}

void ShuffleDices()
{
  for (int i = 0; i < g_game_state.dice_count; ++i)
  {
    g_game_state.dices[i].value = rand() % 6 + 1;
    g_game_state.dices[i].state = State_Shuffle;
  }
}

bool CountScore()
{
  bool match = false;

  for (int value = 1; value <= 6; ++value)
  {
    int count = 0;

    for (int i = 0; i < g_game_state.dice_count; ++i)
    {
      if (g_game_state.dices[i].value == value)
      {
        ++count;
      }
    }

    if (count > 1)
    {
      for (int i = 0; i < g_game_state.dice_count; ++i)
      {
        if (g_game_state.dices[i].value == value)
        {
          g_game_state.dices[i].state = State_Success;
        }
      }

      g_game_state.score += 2 * (count - 1);
      match = true;
    }
  }

  if (match)
  {
    ++g_game_state.successful_attempts;
  }
  else
  {
    for (int i = 0; i < g_game_state.dice_count; ++i)
    {
      g_game_state.dices[i].state = State_Fail;
    }

    ++g_game_state.failed_attempts;
    g_game_state.score -= g_game_state.dice_count;
  }

  return match;
}
