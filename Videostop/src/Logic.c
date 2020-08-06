#include "Logic.h"

#include <stdbool.h>  // bool, false, true
#include <stdio.h>    // snprintf
#include <stdlib.h>   // srand, rand
#include <time.h>     // time

#include "CommonCore.h"

#include "Audio.h"
#include "Config.h"
#include "Graphics.h"
#include "Input.h"
#include "World.h"

static void UpdateWindowTitle();

static void ShuffleDices(enum DiceState state);
static bool CountScore();

void L_Start()
{
  UpdateWindowTitle();

  srand((unsigned int) time(NULL));

  L_Restart();
}

void L_Restart()
{
  g_world.state = WorldState_Setup;

  g_world.dice_count = g_config.dice_count;

  ShuffleDices(DiceState_Setup);

  g_world.statusBar.state = StatusBarState_Show;

  g_world.successful_attempts = 0;
  g_world.failed_attempts = 0;
  g_world.score = 0;
}

void L_Update()
{
  static float idle_time;
  static float stop_time;
  static float statusbar_time;

  if (g_world.dice_count != g_config.dice_count)
  {
    I_Restart();
    L_Restart();
    A_Restart();
    G_Restart();

    idle_time = 0.0f;
    stop_time = 0.0f;
    statusbar_time = 0.0f;
    return;
  }

  if (I_ResetKey())
  {
    I_Restart();
    L_Restart();
    A_Restart();
    // G_Restart();

    idle_time = 0.0f;
    stop_time = 0.0f;
    statusbar_time = 0.0f;
    return;
  }

  bool const control_key = I_ControlKey();
  bool const control_button = I_ControlButton();
  bool const size_up_key = I_SizeUpKey();
  bool const size_down_key = I_SizeDownKey();
  bool const speed_up_key = I_SpeedUpKey();
  bool const speed_down_key = I_SpeedDownKey();

  switch (g_world.state)
  {
    case WorldState_Setup:
      idle_time += UPDATE_PERIOD_MS;
      statusbar_time += UPDATE_PERIOD_MS;

      if (control_key || control_button)
      {
        A_PlaySound(Sound_Start);
        ShuffleDices(DiceState_Idle);

        g_world.state = WorldState_Idle;
        idle_time = 0.0f;
        break;
      }

      if (size_up_key)
      {
        if (g_config.dice_count < MAX_DICE_COUNT)
        {
          ++g_config.dice_count;
        }
      }

      if (size_down_key)
      {
        if (g_config.dice_count > MIN_DICE_COUNT)
        {
          --g_config.dice_count;
        }
      }

      if (speed_up_key)
      {
        g_config.shuffle_frequency += SHUFFLE_FREQUENCY_STEP_HZ;
        if (g_config.shuffle_frequency > MAX_SHUFFLE_FREQUENCY_HZ)
        {
          g_config.shuffle_frequency = MAX_SHUFFLE_FREQUENCY_HZ;
        }

        UpdateWindowTitle();
      }

      if (speed_down_key)
      {
        g_config.shuffle_frequency -= SHUFFLE_FREQUENCY_STEP_HZ;
        if (g_config.shuffle_frequency < MIN_SHUFFLE_FREQUENCY_HZ)
        {
          g_config.shuffle_frequency = MIN_SHUFFLE_FREQUENCY_HZ;
        }

        UpdateWindowTitle();
      }

      if (idle_time >= 1000.0f / g_config.shuffle_frequency)
      {
        A_PlaySound(Sound_Shuffle);
        ShuffleDices(DiceState_Setup);

        idle_time = 0.0f;
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
      idle_time += UPDATE_PERIOD_MS;

      if (control_key || control_button)
      {
        if (CountScore())
        {
          A_PlaySound(Sound_Success);

          g_world.state = WorldState_Success;
          stop_time = 0.0f;
        }
        else
        {
          A_PlaySound(Sound_Fail);

          g_world.state = WorldState_Fail;
          stop_time = 0.0f;
        }
        break;
      }

      if (idle_time >= 1000.0f / g_config.shuffle_frequency)
      {
        A_PlaySound(Sound_Shuffle);
        ShuffleDices(DiceState_Idle);

        idle_time = 0.0f;
      }
      break;

    case WorldState_Success:
    case WorldState_Fail:
      stop_time += UPDATE_PERIOD_MS;

      if (stop_time >= STOP_TIME_MS)
      {
        A_PlaySound(Sound_Shuffle);
        ShuffleDices(DiceState_Idle);

        g_world.state = WorldState_Idle;
        idle_time = 0.0f;
      }
      break;
  }
}

void L_Stop()
{
}

void UpdateWindowTitle()
{
  char title[30];
  snprintf(title, sizeof(title), "Videostop @ %.2f Hz", g_config.shuffle_frequency);

  CC_SetWindowTitle(title);
}

void ShuffleDices(enum DiceState const state)
{
  for (int i = 0; i < g_world.dice_count; ++i)
  {
    g_world.dices[i].state = state;
    g_world.dices[i].value = rand() % 6 + 1;
  }
}

bool CountScore()
{
  bool match = false;

  for (int value = 1; value <= 6; ++value)
  {
    int count = 0;

    for (int i = 0; i < g_world.dice_count; ++i)
    {
      if (g_world.dices[i].value == value)
      {
        ++count;
      }
    }

    if (count > 1)
    {
      for (int i = 0; i < g_world.dice_count; ++i)
      {
        if (g_world.dices[i].value == value)
        {
          g_world.dices[i].state = DiceState_Success;
        }
      }

      g_world.score += 2 * (count - 1);
      match = true;
    }
  }

  if (match)
  {
    for (int i = 0; i < g_world.dice_count; ++i)
    {
      if (g_world.dices[i].state != DiceState_Success)
      {
        g_world.dices[i].state = DiceState_Idle;
      }
    }

    ++g_world.successful_attempts;
  }
  else
  {
    for (int i = 0; i < g_world.dice_count; ++i)
    {
      g_world.dices[i].state = DiceState_Fail;
    }

    ++g_world.failed_attempts;
    g_world.score -= g_world.dice_count;
  }

  return match;
}
