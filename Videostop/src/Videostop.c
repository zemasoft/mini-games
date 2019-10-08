// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <stdlib.h>  // EXIT_SUCCESS, strtol

#include <AL/alut.h>
#include <GL/freeglut.h>

#include "GameConfig.h"
#include "Graphics.h"
#include "Input.h"
#include "Logic.h"
#include "Sound.h"

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

  g_game_config.dice_count = DEFAULT_DICE_COUNT;
  g_game_config.shuffle_frequency = DEFAULT_SHUFFLE_FREQUENCY_HZ;

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

      g_game_config.dice_count = (int) dice_count;
    }
  }

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
  glutInitWindowSize(g_game_config.dice_count * DICE_SIZE_PIXELS, DICE_SIZE_PIXELS);
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
