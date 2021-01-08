#include "Input.h"

#include <stddef.h>  // NULL

#include "zge/core.h"

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

static bool s_resetKey;
static bool s_controlKey;
static bool s_controlButton;
static bool s_sizeUpKey;
static bool s_sizeDownKey;
static bool s_speedUpKey;
static bool s_speedDownKey;

#if defined(USE_FREEGLUT)
static void Keyboard(unsigned char key, int x, int y);
static void Special(int key, int x, int y);
static void Mouse(int button, int state, int x, int y);
#endif

#if defined(USE_GLFW)
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
#endif

#if defined(USE_SDL2)
static void ProcessKeyEvent(SDL_Event const* e);
static void ProcessMouseButtonEvent(SDL_Event const* e);
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
  s_resetKey = false;
  s_controlKey = false;
  s_controlButton = false;
  s_sizeUpKey = false;
  s_sizeDownKey = false;
  s_speedUpKey = false;
  s_speedDownKey = false;
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
        ZGE_LeaveGameLoop();
        break;
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        ProcessKeyEvent(&e);
        break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        ProcessMouseButtonEvent(&e);
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
#endif
}

bool I_ResetKey()
{
  bool const res = s_resetKey;

  s_resetKey = false;

  return res;
}

bool I_ControlKey()
{
  bool const res = s_controlKey;

  s_controlKey = false;

  return res;
}

bool I_ControlButton()
{
  bool const res = s_controlButton;

  s_controlButton = false;

  return res;
}

bool I_SizeUpKey()
{
  bool const res = s_sizeUpKey;

  s_sizeUpKey = false;

  return res;
}

bool I_SizeDownKey()
{
  bool const res = s_sizeDownKey;

  s_sizeDownKey = false;

  return res;
}

bool I_SpeedUpKey()
{
  bool const res = s_speedUpKey;

  s_speedUpKey = false;

  return res;
}

bool I_SpeedDownKey()
{
  bool const res = s_speedDownKey;

  s_speedDownKey = false;

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
      s_controlKey = true;
      break;
    case 27:  // Escape
      ZGE_LeaveGameLoop();
      break;
    case 32:  // Space
      s_controlKey = true;
      break;
    case 114:  // r
      s_resetKey = true;
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
      s_sizeUpKey = true;
      break;
    case GLUT_KEY_LEFT:
      s_sizeDownKey = true;
      break;
    case GLUT_KEY_UP:
      s_speedUpKey = true;
      break;
    case GLUT_KEY_DOWN:
      s_speedDownKey = true;
      break;
  }
}

void Mouse(int const button, int const state, int const x, int const y)
{
  (void) x;
  (void) y;

  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    s_controlButton = true;
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
        s_controlKey = true;
        break;
      case GLFW_KEY_ESCAPE:
        ZGE_LeaveGameLoop();
        break;
      case GLFW_KEY_R:
        s_resetKey = true;
        break;
      case GLFW_KEY_RIGHT:
        s_sizeUpKey = true;
        break;
      case GLFW_KEY_LEFT:
        s_sizeDownKey = true;
        break;
      case GLFW_KEY_UP:
        s_speedUpKey = true;
        break;
      case GLFW_KEY_DOWN:
        s_speedDownKey = true;
        break;
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
    s_controlButton = true;
  }
}

#endif

#if defined(USE_SDL2)

void ProcessKeyEvent(SDL_Event const* const e)
{
  if (e->type == SDL_KEYDOWN)
  {
    if (e->key.keysym.scancode == SDL_SCANCODE_RETURN)
    {
      s_controlKey = true;
    }
    else if (e->key.keysym.scancode == SDL_SCANCODE_SPACE)
    {
      s_controlKey = true;
    }
    else if (e->key.keysym.scancode == SDL_SCANCODE_ESCAPE)
    {
      ZGE_LeaveGameLoop();
    }
    else if (e->key.keysym.scancode == SDL_SCANCODE_R)
    {
      s_resetKey = true;
    }
    else if (e->key.keysym.scancode == SDL_SCANCODE_RIGHT)
    {
      s_sizeUpKey = true;
    }
    else if (e->key.keysym.scancode == SDL_SCANCODE_LEFT)
    {
      s_sizeDownKey = true;
    }
    else if (e->key.keysym.scancode == SDL_SCANCODE_UP)
    {
      s_speedUpKey = true;
    }
    else if (e->key.keysym.scancode == SDL_SCANCODE_DOWN)
    {
      s_speedDownKey = true;
    }
  }
}

void ProcessMouseButtonEvent(SDL_Event const* const e)
{
  if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
  {
    s_controlButton = true;
  }
}

#endif
