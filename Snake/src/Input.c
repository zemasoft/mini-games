// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Input.h"

#include <assert.h>

#include <GLFW/glfw3.h>

extern GLFWwindow* g_window;

static bool s_keys_pressed[GLFW_KEY_LAST];

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void I_Start()
{
  glfwSetKeyCallback(g_window, KeyCallback);
}

void I_Update()
{
  glfwPollEvents();
}

void I_Stop()
{
  glfwSetKeyCallback(g_window, NULL);
}

bool I_IsKeyPressed(int key)
{
  assert(key >= 0 && key < GLFW_KEY_LAST);

  return s_keys_pressed[key];
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  (void) window;
  (void) scancode;
  (void) mods;

  if (key < 0 || key >= GLFW_KEY_LAST)
  {
    return;
  }

  if (action == GLFW_PRESS)
  {
    s_keys_pressed[key] = true;
  }
  else if (action == GLFW_RELEASE)
  {
    s_keys_pressed[key] = false;
  }
}
