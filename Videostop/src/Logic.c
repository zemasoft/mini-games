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
#include "Graphics.h"
#include "Input.h"
#include "Sound.h"

#define IDLE_TIME_STEP_MS 100

#define MAX_STOP_TIME_MS 1000

static void ShuffleDices(enum State state);
static bool CountScore();

void L_Start()
{
  srand((unsigned int) time(NULL));

  L_Restart();
}

void L_Update()
{
  static int before;
  static int idle_time;
  static int stop_time;

  int now = glutGet(GLUT_ELAPSED_TIME);
  int elapsed = now - before;
  before = now;

  if (g_game_state.dice_count != g_game_state.dice_count_sp)
  {
    I_Restart();
    L_Restart();
    S_Restart();
    G_Restart();

    idle_time = 0;
    stop_time = 0;
    return;
  }

  if (I_ResetKey())
  {
    I_Restart();
    L_Restart();
    S_Restart();

    idle_time = 0;
    stop_time = 0;
    return;
  }

  bool control_key = I_ControlKey();
  bool control_button = I_ControlButton();

  bool size_up_key = I_SizeUpKey();
  bool size_down_key = I_SizeDownKey();
  bool speed_up_key = I_SpeedUpKey();
  bool speed_down_key = I_SpeedDownKey();

  switch (g_game_state.state)
  {
    case State_Setup:
      if (size_up_key)
      {
        if (g_game_state.dice_count_sp < MAX_DICE_COUNT)
        {
          ++g_game_state.dice_count_sp;
        }
      }

      if (size_down_key)
      {
        if (g_game_state.dice_count_sp > MIN_DICE_COUNT)
        {
          --g_game_state.dice_count_sp;
        }
      }

      if (speed_up_key)
      {
        g_game_state.idle_time -= IDLE_TIME_STEP_MS;
        if (g_game_state.idle_time < MIN_IDLE_TIME_MS)
        {
          g_game_state.idle_time = MIN_IDLE_TIME_MS;
        }
      }

      if (speed_down_key)
      {
        g_game_state.idle_time += IDLE_TIME_STEP_MS;
        if (g_game_state.idle_time > MAX_IDLE_TIME_MS)
        {
          g_game_state.idle_time = MAX_IDLE_TIME_MS;
        }
      }

    case State_Idle:
      idle_time += elapsed;

      if (control_key || control_button)
      {
        if (CountScore())
        {
          S_PlaySound(Sound_Success);

          g_game_state.state = State_Success;
          stop_time = 0;
        }
        else
        {
          S_PlaySound(Sound_Fail);

          g_game_state.state = State_Fail;
          stop_time = 0;
        }
        break;
      }

      if (idle_time >= g_game_state.idle_time)
      {
        S_PlaySound(Sound_Shuffle);
        ShuffleDices(g_game_state.state);

        idle_time = 0;
      }
      break;

    case State_Success:
    case State_Fail:
      stop_time += elapsed;

      if (stop_time >= MAX_STOP_TIME_MS)
      {
        S_PlaySound(Sound_Shuffle);
        ShuffleDices(State_Idle);

        g_game_state.state = State_Idle;
        idle_time = 0;
      }
      break;
  }
}

void L_Stop()
{
}

void L_Restart()
{
  g_game_state.dice_count = g_game_state.dice_count_sp;

  ShuffleDices(State_Setup);

  g_game_state.state = State_Setup;

  g_game_state.successful_attempts = 0;
  g_game_state.failed_attempts = 0;

  g_game_state.score = 0;
}

void ShuffleDices(enum State state)
{
  for (int i = 0; i < g_game_state.dice_count; ++i)
  {
    g_game_state.dices[i].value = rand() % 6 + 1;
    g_game_state.dices[i].state = state;
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
    for (int i = 0; i < g_game_state.dice_count; ++i)
    {
      if (g_game_state.dices[i].state != State_Success)
      {
        g_game_state.dices[i].state = State_Idle;
      }
    }

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
