// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <stdlib.h>  // EXIT_FAILURE, EXIT_SUCCESS

#include <AL/alut.h>
#include <GLFW/glfw3.h>

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

  alutInit(&argc, argv);

  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  g_window = glfwCreateWindow(500, 500, "Snake", NULL, NULL);
  if (!g_window)
  {
    alutExit();
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

  alutExit();
  glfwTerminate();
  return EXIT_SUCCESS;
}
