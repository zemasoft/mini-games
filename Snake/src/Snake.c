// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <stdlib.h>  // EXIT_FAILURE, EXIT_SUCCESS, strtol

#if defined(USE_FREEALUT)
#include <AL/alut.h>
#endif

#include <GLFW/glfw3.h>

#include "Config.h"
#include "Graphics.h"
#include "Input.h"
#include "Logic.h"
#include "Sound.h"

GLFWwindow* g_window;

int main(int argc, char** argv)
{
  if (!glfwInit())
  {
    return EXIT_FAILURE;
  }

#if defined(USE_FREEALUT)
  alutInit(&argc, argv);
#endif

  g_config.ground.size.x = DEFAULT_X_SIZE;
  g_config.ground.size.y = DEFAULT_Y_SIZE;

  if (argc > 1)
  {
    char* end;
    long y_size = strtol(argv[1], &end, 10);

    if (y_size != 0 && *end == '\0')
    {
      if (y_size < MIN_Y_SIZE)
      {
        y_size = MIN_Y_SIZE;
      }
      else if (y_size > MAX_Y_SIZE)
      {
        y_size = MAX_Y_SIZE;
      }

      g_config.ground.size.x = (int) y_size;
      g_config.ground.size.y = (int) y_size;
    }
  }

  if (argc > 2)
  {
    char* end;
    long x_size = strtol(argv[2], &end, 10);

    if (x_size != 0 && *end == '\0')
    {
      if (x_size < MIN_X_SIZE)
      {
        x_size = MIN_X_SIZE;
      }
      else if (x_size > MAX_X_SIZE)
      {
        x_size = MAX_X_SIZE;
      }

      g_config.ground.size.x = (int) x_size;
    }
  }

  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  g_window =
      glfwCreateWindow((int) g_config.ground.size.x * FIELD_SIZE_PIXELS,
                       (int) g_config.ground.size.y * FIELD_SIZE_PIXELS, "Snake", NULL, NULL);
  if (!g_window)
  {
#if defined(USE_FREEALUT)
    alutExit();
#endif

    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(g_window);

  I_Start();
  L_Start();
  S_Start();
  G_Start();

  while (!glfwWindowShouldClose(g_window))
  {
    I_Update();
    L_Update();
    G_Update();
  }

  I_Stop();
  L_Stop();
  S_Stop();
  G_Stop();

#if defined(USE_FREEALUT)
  alutExit();
#endif

  glfwTerminate();
  return EXIT_SUCCESS;
}
