#include "Logic.h"

#include <stdbool.h>  // bool, false, true
#include <stddef.h>   // NULL
#include <stdio.h>    // snprintf
#include <stdlib.h>   // srand, rand
#include <time.h>     // time

#include "zge/core.h"

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

  g_world.diceCount = g_config.diceCount;

  ShuffleDices(DiceState_Setup);

  g_world.statusBar.state = StatusBarState_Show;

  g_world.successfulAttempts = 0;
  g_world.failedAttempts = 0;
  g_world.score = 0;
}

void L_Update()
{
  static float idleTime;
  static float stopTime;
  static float statusbarTime;

  if (g_world.diceCount != g_config.diceCount)
  {
    I_Restart();
    L_Restart();
    A_Restart();
    G_Restart();

    idleTime = 0.0f;
    stopTime = 0.0f;
    statusbarTime = 0.0f;
    return;
  }

  if (I_ResetKey())
  {
    I_Restart();
    L_Restart();
    A_Restart();
    // G_Restart();

    idleTime = 0.0f;
    stopTime = 0.0f;
    statusbarTime = 0.0f;
    return;
  }

  bool const controlKey = I_ControlKey();
  bool const controlButton = I_ControlButton();
  bool const sizeUpKey = I_SizeUpKey();
  bool const sizeDownKey = I_SizeDownKey();
  bool const speedUpKey = I_SpeedUpKey();
  bool const speedDownKey = I_SpeedDownKey();

  switch (g_world.state)
  {
    case WorldState_Setup:
      idleTime += UPDATE_PERIOD_MS;
      statusbarTime += UPDATE_PERIOD_MS;

      if (controlKey || controlButton)
      {
        A_PlaySound(Sound_Start);
        ShuffleDices(DiceState_Idle);

        g_world.state = WorldState_Idle;
        idleTime = 0.0f;
        break;
      }

      if (sizeUpKey)
      {
        if (g_config.diceCount < MAX_DICE_COUNT)
        {
          ++g_config.diceCount;
        }
      }

      if (sizeDownKey)
      {
        if (g_config.diceCount > MIN_DICE_COUNT)
        {
          --g_config.diceCount;
        }
      }

      if (speedUpKey)
      {
        g_config.shuffleFrequency += SHUFFLE_FREQUENCY_STEP_HZ;
        if (g_config.shuffleFrequency > MAX_SHUFFLE_FREQUENCY_HZ)
        {
          g_config.shuffleFrequency = MAX_SHUFFLE_FREQUENCY_HZ;
        }

        UpdateWindowTitle();
      }

      if (speedDownKey)
      {
        g_config.shuffleFrequency -= SHUFFLE_FREQUENCY_STEP_HZ;
        if (g_config.shuffleFrequency < MIN_SHUFFLE_FREQUENCY_HZ)
        {
          g_config.shuffleFrequency = MIN_SHUFFLE_FREQUENCY_HZ;
        }

        UpdateWindowTitle();
      }

      if (idleTime >= 1000.0f / g_config.shuffleFrequency)
      {
        A_PlaySound(Sound_Shuffle);
        ShuffleDices(DiceState_Setup);

        idleTime = 0.0f;
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
      idleTime += UPDATE_PERIOD_MS;

      if (controlKey || controlButton)
      {
        if (CountScore())
        {
          A_PlaySound(Sound_Success);

          g_world.state = WorldState_Success;
          stopTime = 0.0f;
        }
        else
        {
          A_PlaySound(Sound_Fail);

          g_world.state = WorldState_Fail;
          stopTime = 0.0f;
        }
        break;
      }

      if (idleTime >= 1000.0f / g_config.shuffleFrequency)
      {
        A_PlaySound(Sound_Shuffle);
        ShuffleDices(DiceState_Idle);

        idleTime = 0.0f;
      }
      break;

    case WorldState_Success:
    case WorldState_Fail:
      stopTime += UPDATE_PERIOD_MS;

      if (stopTime >= STOP_TIME_MS)
      {
        A_PlaySound(Sound_Shuffle);
        ShuffleDices(DiceState_Idle);

        g_world.state = WorldState_Idle;
        idleTime = 0.0f;
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
  snprintf(title, sizeof(title), "Videostop @ %.2f Hz", g_config.shuffleFrequency);

  ZGE_SetWindowTitle(title);
}

void ShuffleDices(enum DiceState const state)
{
  for (int i = 0; i < g_world.diceCount; ++i)
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

    for (int i = 0; i < g_world.diceCount; ++i)
    {
      if (g_world.dices[i].value == value)
      {
        ++count;
      }
    }

    if (count > 1)
    {
      for (int i = 0; i < g_world.diceCount; ++i)
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
    for (int i = 0; i < g_world.diceCount; ++i)
    {
      if (g_world.dices[i].state != DiceState_Success)
      {
        g_world.dices[i].state = DiceState_Idle;
      }
    }

    ++g_world.successfulAttempts;
  }
  else
  {
    for (int i = 0; i < g_world.diceCount; ++i)
    {
      g_world.dices[i].state = DiceState_Fail;
    }

    ++g_world.failedAttempts;
    g_world.score -= g_world.diceCount;
  }

  return match;
}
