// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Input.h"

#include <stddef.h>  // size_t

#if defined(USE_FREEGLUT)
#include <GL/freeglut.h>
#endif

#if defined(USE_GLFW)
#include <GLFW/glfw3.h>
#endif

#if defined(USE_SDL2)
#include <SDL2/SDL.h>
#endif

#include "Config.h"

#if defined(USE_GLFW)
extern GLFWwindow* g_window;
#endif

#if defined(USE_SDL2)
extern bool g_quit;
#endif

static bool s_reset_key;
static bool s_control_key;
static bool s_control_button;
static int s_control_x;
static int s_control_y;

static int s_direction_keys[DIRECTION_KEYS_SIZE];
static size_t s_top;
static size_t s_bot;

#if defined(USE_FREEGLUT)
static void Keyboard(unsigned char key, int x, int y);
static void Special(int key, int x, int y);
static void Mouse(int button, int state, int x, int y);
#endif

#if defined(USE_GLFW)
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
#endif

#if defined(USE_SDL2)
static void ProcessKeyEvent(SDL_Event const* e);
static void ProcessMouseButtonEvent(SDL_Event const* e);
static void ProcessMouseMotionEvent(SDL_Event const* e);
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
  glfwSetCursorPosCallback(g_window, CursorPosCallback);
#endif
}

void I_Restart()
{
  s_reset_key = false;
  s_control_key = false;
  s_control_button = false;

  s_top = 0;
  s_bot = 0;
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
    switch (e.type)
    {
      case SDL_QUIT:
        g_quit = true;
        break;
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        ProcessKeyEvent(&e);
        break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        ProcessMouseButtonEvent(&e);
        break;
      case SDL_MOUSEMOTION:
        ProcessMouseMotionEvent(&e);
        break;
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
  glfwSetCursorPosCallback(g_window, NULL);
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

bool I_ControlButton(int* const x, int* const y)
{
  bool const res = s_control_button;

  s_control_button = false;

  if (res)
  {
    *x = s_control_x;
    *y = s_control_y;
  }

  return res;
}

int I_PopDirectionKey()
{
  if (s_bot == s_top)
  {
    return -1;
  }

  int const key = s_direction_keys[s_bot++];

  if (s_bot > DIRECTION_KEYS_SIZE - 1)
  {
    s_bot = 0;
  }

  return key;
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
    case GLUT_KEY_LEFT:
      s_direction_keys[s_top++] = KEY_LEFT;
      break;
    case GLUT_KEY_RIGHT:
      s_direction_keys[s_top++] = KEY_RIGHT;
      break;
    case GLUT_KEY_DOWN:
      s_direction_keys[s_top++] = KEY_DOWN;
      break;
    case GLUT_KEY_UP:
      s_direction_keys[s_top++] = KEY_UP;
      break;
  }

  if (s_top > DIRECTION_KEYS_SIZE - 1)
  {
    s_top = 0;
  }
}

void Mouse(int const button, int const state, int const x, int const y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    s_control_button = true;
    s_control_x = x;
    s_control_y = y;
  }
}

#endif

#if defined(USE_GLFW)

void KeyCallback(GLFWwindow* const window, int const key, int const scancode, int const action,
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
      case GLFW_KEY_LEFT:
        s_direction_keys[s_top++] = KEY_LEFT;
        break;
      case GLFW_KEY_RIGHT:
        s_direction_keys[s_top++] = KEY_RIGHT;
        break;
      case GLFW_KEY_DOWN:
        s_direction_keys[s_top++] = KEY_DOWN;
        break;
      case GLFW_KEY_UP:
        s_direction_keys[s_top++] = KEY_UP;
        break;
    }

    if (s_top > DIRECTION_KEYS_SIZE - 1)
    {
      s_top = 0;
    }
  }
}

void MouseButtonCallback(GLFWwindow* const window, int const button, int const action,
                         int const mods)
{
  (void) window;
  (void) mods;

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
  {
    s_control_button = true;
  }
}

void CursorPosCallback(GLFWwindow* const window, double const xpos, double const ypos)
{
  (void) window;

  s_control_x = (int) xpos;
  s_control_y = (int) ypos;
}

#endif

#if defined(USE_SDL2)

void ProcessKeyEvent(SDL_Event const* const e)
{
  if (e->type == SDL_KEYDOWN)
  {
    if (e->key.keysym.scancode == SDL_SCANCODE_RETURN)
    {
      s_control_key = true;
    }
    else if (e->key.keysym.scancode == SDL_SCANCODE_SPACE)
    {
      s_control_key = true;
    }
    else if (e->key.keysym.scancode == SDL_SCANCODE_ESCAPE)
    {
      g_quit = true;
    }
    else if (e->key.keysym.scancode == SDL_SCANCODE_R)
    {
      s_reset_key = true;
    }
    else if (e->key.keysym.scancode == SDL_SCANCODE_LEFT)
    {
      s_direction_keys[s_top++] = KEY_LEFT;
    }
    else if (e->key.keysym.scancode == SDL_SCANCODE_RIGHT)
    {
      s_direction_keys[s_top++] = KEY_RIGHT;
    }
    else if (e->key.keysym.scancode == SDL_SCANCODE_DOWN)
    {
      s_direction_keys[s_top++] = KEY_DOWN;
    }
    else if (e->key.keysym.scancode == SDL_SCANCODE_UP)
    {
      s_direction_keys[s_top++] = KEY_UP;
    }

    if (s_top > DIRECTION_KEYS_SIZE - 1)
    {
      s_top = 0;
    }
  }
}

void ProcessMouseButtonEvent(SDL_Event const* const e)
{
  if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
  {
    s_control_button = true;
  }
}

void ProcessMouseMotionEvent(SDL_Event const* e)
{
  s_control_x = e->motion.x;
  s_control_y = e->motion.y;
}

#endif
