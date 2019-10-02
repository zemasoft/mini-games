// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Input.h"

#include <GLFW/glfw3.h>

#define CONTROL_KEYS_SIZE 4

extern GLFWwindow* g_window;

static bool s_reset_key;
static bool s_pause_key;

static int s_control_keys[CONTROL_KEYS_SIZE];
static size_t s_top;
static size_t s_bot;

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void I_Start()
{
  I_Restart();

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

void I_Restart()
{
  s_reset_key = false;
  s_pause_key = false;

  s_top = 0;
  s_bot = 0;
}

bool I_ResetKey()
{
  bool res = s_reset_key;

  s_reset_key = false;

  return res;
}

bool I_PauseKey()
{
  bool res = s_pause_key;

  s_pause_key = false;

  return res;
}

int I_PopControlKey()
{
  if (s_bot == s_top)
  {
    return -1;
  }

  int key = s_control_keys[s_bot++];

  if (s_bot > CONTROL_KEYS_SIZE - 1)
  {
    s_bot = 0;
  }

  return key;
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
    switch (key)
    {
      case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(g_window, GLFW_TRUE);
        break;

      case GLFW_KEY_R:
        s_reset_key = true;
        break;

      case GLFW_KEY_P:
        s_pause_key = true;
        break;

      case GLFW_KEY_LEFT:
      case GLFW_KEY_RIGHT:
      case GLFW_KEY_DOWN:
      case GLFW_KEY_UP:
        s_control_keys[s_top++] = key;

        if (s_top > CONTROL_KEYS_SIZE - 1)
        {
          s_top = 0;
        }
        break;
    }
  }
}
