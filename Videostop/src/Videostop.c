// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <stdlib.h>  // EXIT_SUCCESS, strtol

#include <AL/alut.h>
#include <GL/freeglut.h>

#include "GameState.h"
#include "Graphics.h"
#include "Input.h"
#include "Logic.h"
#include "Sound.h"

#define DICE_SIZE_PIXELS 200

static void Start(int argc, char** argv);
static void Update();
static void Stop();

int main(int argc, char** argv)
{
  Start(argc, argv);
  Stop();

  return EXIT_SUCCESS;
}

void Start(int argc, char** argv)
{
  glutInit(&argc, argv);
  alutInit(&argc, argv);

  g_game_state.dice_count_sp = DEFAULT_DICE_COUNT;

  if (argc > 1)
  {
    char* end;
    long dice_count = strtol(argv[1], &end, 10);

    if (dice_count != 0 && *end == '\0')
    {
      if (dice_count < MIN_DICE_COUNT)
      {
        dice_count = MIN_DICE_COUNT;
      }
      else if (dice_count > MAX_DICE_COUNT)
      {
        dice_count = MAX_DICE_COUNT;
      }

      g_game_state.dice_count_sp = (int) dice_count;
    }
  }

  g_game_state.idle_time = DEFAULT_IDLE_TIME_MS;

  if (argc > 2)
  {
    char* end;
    long idle_time = strtol(argv[2], &end, 10);

    if (idle_time != 0 && *end == '\0')
    {
      if (idle_time < MIN_IDLE_TIME_MS)
      {
        idle_time = MIN_IDLE_TIME_MS;
      }
      else if (idle_time > MAX_IDLE_TIME_MS)
      {
        idle_time = MAX_IDLE_TIME_MS;
      }

      g_game_state.idle_time = (int) idle_time;
    }
  }

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
  glutInitWindowSize(g_game_state.dice_count_sp * DICE_SIZE_PIXELS, DICE_SIZE_PIXELS);
  glutCreateWindow("Videostop");

  I_Start();
  L_Start();
  S_Start();
  G_Start();

  // glutDisplayFunc(&G_Update);
  glutIdleFunc(&Update);

  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  glutMainLoop();
}

void Update()
{
  I_Update();
  L_Update();
  G_Update();
}

void Stop()
{
  I_Stop();
  L_Stop();
  S_Stop();
  G_Stop();

  // glutDisplayFunc(NULL);
  glutIdleFunc(NULL);

  alutExit();
  glutExit();
}
