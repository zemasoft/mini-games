// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Graphics.h"

#include <GLFW/glfw3.h>

extern GLFWwindow* g_window;

void G_Start()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);

  glMatrixMode(GL_MODELVIEW);
}

void G_Update()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  // clang-format off
  glBegin(GL_POLYGON);
    glVertex2f(0.25f, 0.75f);
    glVertex2f(0.25f, 0.25f);
    glVertex2f(0.75f, 0.25f);
    glVertex2f(0.75f, 0.75f);
  glEnd();
  // clang-format on

  glfwSwapBuffers(g_window);
}

void G_Stop()
{
}
