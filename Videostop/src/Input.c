// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Input.h"

#if defined(USE_FREEGLUT)
#include <GL/freeglut.h>
#endif

#if defined(USE_GLFW)
#include <GLFW/glfw3.h>
#endif

#if defined(USE_SDL2)
#include <SDL2/SDL.h>
#endif

#if defined(USE_GLFW)
extern GLFWwindow* g_window;
#endif

#if defined(USE_SDL2)
extern bool g_quit;
#endif

static bool s_reset_key;
static bool s_control_key;
static bool s_control_button;
static bool s_size_up_key;
static bool s_size_down_key;
static bool s_speed_up_key;
static bool s_speed_down_key;

#if defined(USE_FREEGLUT)
static void Keyboard(unsigned char key, int x, int y);
static void Special(int key, int x, int y);
static void Mouse(int button, int state, int x, int y);
#endif

#if defined(USE_GLFW)
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
#endif

void I_Start()
{
  I_Restart();

#if defined(USE_FREEGLUT)
  glutKeyboardFunc(&Keyboard);
  glutSpecialFunc(&Special);
  glutMouseFunc(&Mouse);
#endif

#if defined(USE_GLFW)
  glfwSetKeyCallback(g_window, KeyCallback);
  glfwSetMouseButtonCallback(g_window, MouseButtonCallback);
#endif
}

void I_Restart()
{
  s_reset_key = false;
  s_control_key = false;
  s_control_button = false;
  s_size_up_key = false;
  s_size_down_key = false;
  s_speed_up_key = false;
  s_speed_down_key = false;
}

void I_Update()
{
#if defined(USE_GLFW)
  glfwPollEvents();
#endif

#if defined(USE_SDL2)
  SDL_Event e;
  while (SDL_PollEvent(&e) != 0)
  {
    if (e.type == SDL_QUIT)
    {
      g_quit = true;
    }
  }
#endif
}

void I_Stop()
{
#if defined(USE_FREEGLUT)
  glutKeyboardFunc(NULL);
  glutSpecialFunc(NULL);
  glutMouseFunc(NULL);
#endif

#if defined(USE_GLFW)
  glfwSetKeyCallback(g_window, NULL);
  glfwSetMouseButtonCallback(g_window, NULL);
#endif
}

bool I_ResetKey()
{
  bool const res = s_reset_key;

  s_reset_key = false;

  return res;
}

bool I_ControlKey()
{
  bool const res = s_control_key;

  s_control_key = false;

  return res;
}

bool I_ControlButton()
{
  bool const res = s_control_button;

  s_control_button = false;

  return res;
}

bool I_SizeUpKey()
{
  bool const res = s_size_up_key;

  s_size_up_key = false;

  return res;
}

bool I_SizeDownKey()
{
  bool const res = s_size_down_key;

  s_size_down_key = false;

  return res;
}

bool I_SpeedUpKey()
{
  bool const res = s_speed_up_key;

  s_speed_up_key = false;

  return res;
}

bool I_SpeedDownKey()
{
  bool const res = s_speed_down_key;

  s_speed_down_key = false;

  return res;
}

#if defined(USE_FREEGLUT)

void Keyboard(unsigned char const key, int const x, int const y)
{
  (void) x;
  (void) y;

  switch (key)
  {
    case 13:  // Enter
      s_control_key = true;
      break;
    case 27:  // Escape
      glutLeaveMainLoop();
      break;
    case 32:  // Space
      s_control_key = true;
      break;
    case 114:  // r
      s_reset_key = true;
      break;
  }
}

void Special(int const key, int const x, int const y)
{
  (void) x;
  (void) y;

  switch (key)
  {
    case GLUT_KEY_RIGHT:
      s_size_up_key = true;
      break;
    case GLUT_KEY_LEFT:
      s_size_down_key = true;
      break;
    case GLUT_KEY_UP:
      s_speed_up_key = true;
      break;
    case GLUT_KEY_DOWN:
      s_speed_down_key = true;
      break;
  }
}

void Mouse(int const button, int const state, int const x, int const y)
{
  (void) x;
  (void) y;

  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    s_control_button = true;
  }
}

#endif

#if defined(USE_GLFW)

void KeyCallback(GLFWwindow* window, int const key, int const scancode, int const action,
                 int const mods)
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
      case GLFW_KEY_ENTER:
      case GLFW_KEY_SPACE:
        s_control_key = true;
        break;
      case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(g_window, GLFW_TRUE);
        break;
      case GLFW_KEY_R:
        s_reset_key = true;
        break;
      case GLFW_KEY_RIGHT:
        s_size_up_key = true;
        break;
      case GLFW_KEY_LEFT:
        s_size_down_key = true;
        break;
      case GLFW_KEY_UP:
        s_speed_up_key = true;
        break;
      case GLFW_KEY_DOWN:
        s_speed_down_key = true;
        break;
    }
  }
}

void MouseButtonCallback(GLFWwindow* window, int const button, int const action, int const mods)
{
  (void) window;
  (void) mods;

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
  {
    s_control_button = true;
  }
}

#endif
