// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Logic.h"

#include <GLFW/glfw3.h>

#include "Input.h"
#include "Sound.h"

extern GLFWwindow* g_window;

void L_Start()
{
}

void L_Update()
{
  if (I_IsKeyPressed(GLFW_KEY_ESCAPE))
  {
    glfwSetWindowShouldClose(g_window, GLFW_TRUE);
    return;
  }
}

void L_Stop()
{
}
