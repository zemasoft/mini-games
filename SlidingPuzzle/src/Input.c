#include "Input.h"

#include <stddef.h>  // NULL, size_t

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

#include "Config.h"

#if defined(USE_GLFW)
extern GLFWwindow* g_window;
#endif

static bool s_resetKey;
static bool s_controlKey;
static bool s_controlButton;
static int s_controlX;
static int s_controlY;

static int s_directionKeys[DIRECTION_KEYA_SIZE];
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
  s_resetKey = false;
  s_controlKey = false;
  s_controlButton = false;

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

bool I_ControlButton(int* const x, int* const y)
{
  bool const res = s_controlButton;

  s_controlButton = false;

  if (res)
  {
    *x = s_controlX;
    *y = s_controlY;
  }

  return res;
}

int I_PopDirectionKey()
{
  if (s_bot == s_top)
  {
    return -1;
  }

  int const key = s_directionKeys[s_bot++];

  if (s_bot > DIRECTION_KEYA_SIZE - 1)
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
    case GLUT_KEY_LEFT:
      s_directionKeys[s_top++] = KEY_LEFT;
      break;
    case GLUT_KEY_RIGHT:
      s_directionKeys[s_top++] = KEY_RIGHT;
      break;
    case GLUT_KEY_DOWN:
      s_directionKeys[s_top++] = KEY_DOWN;
      break;
    case GLUT_KEY_UP:
      s_directionKeys[s_top++] = KEY_UP;
      break;
  }

  if (s_top > DIRECTION_KEYA_SIZE - 1)
  {
    s_top = 0;
  }
}

void Mouse(int const button, int const state, int const x, int const y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    s_controlButton = true;
    s_controlX = x;
    s_controlY = y;
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
      case GLFW_KEY_LEFT:
        s_directionKeys[s_top++] = KEY_LEFT;
        break;
      case GLFW_KEY_RIGHT:
        s_directionKeys[s_top++] = KEY_RIGHT;
        break;
      case GLFW_KEY_DOWN:
        s_directionKeys[s_top++] = KEY_DOWN;
        break;
      case GLFW_KEY_UP:
        s_directionKeys[s_top++] = KEY_UP;
        break;
    }

    if (s_top > DIRECTION_KEYA_SIZE - 1)
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
    s_controlButton = true;
  }
}

void CursorPosCallback(GLFWwindow* const window, double const xpos, double const ypos)
{
  (void) window;

  s_controlX = (int) xpos;
  s_controlY = (int) ypos;
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
    else if (e->key.keysym.scancode == SDL_SCANCODE_LEFT)
    {
      s_directionKeys[s_top++] = KEY_LEFT;
    }
    else if (e->key.keysym.scancode == SDL_SCANCODE_RIGHT)
    {
      s_directionKeys[s_top++] = KEY_RIGHT;
    }
    else if (e->key.keysym.scancode == SDL_SCANCODE_DOWN)
    {
      s_directionKeys[s_top++] = KEY_DOWN;
    }
    else if (e->key.keysym.scancode == SDL_SCANCODE_UP)
    {
      s_directionKeys[s_top++] = KEY_UP;
    }

    if (s_top > DIRECTION_KEYA_SIZE - 1)
    {
      s_top = 0;
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

void ProcessMouseMotionEvent(SDL_Event const* e)
{
  s_controlX = e->motion.x;
  s_controlY = e->motion.y;
}

#endif
