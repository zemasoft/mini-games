#include "Input.h"

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

static bool s_reset_key;
static bool s_pause_key;

static int s_direction_keys[DIRECTION_KEYA_SIZE];
static size_t s_top;
static size_t s_bot;

#if defined(USE_FREEGLUT)
static void Keyboard(unsigned char key, int x, int y);
static void Special(int key, int x, int y);
#endif

#if defined(USE_GLFW)
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
#endif

#if defined(USE_SDL2)
static void ProcessKeyEvent(SDL_Event const* e);
#endif

void I_Start()
{
  I_Restart();

#if defined(USE_FREEGLUT)
  glutKeyboardFunc(&Keyboard);
  glutSpecialFunc(&Special);
#endif

#if defined(USE_GLFW)
  glfwSetKeyCallback(g_window, KeyCallback);
#endif
}

void I_Restart()
{
  s_reset_key = false;
  s_pause_key = false;

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
    }
  }
#endif
}

void I_Stop()
{
#if defined(USE_FREEGLUT)
  glutKeyboardFunc(NULL);
  glutSpecialFunc(NULL);
#endif

#if defined(USE_GLFW)
  glfwSetKeyCallback(g_window, NULL);
#endif
}

bool I_ResetKey()
{
  bool const res = s_reset_key;

  s_reset_key = false;

  return res;
}

bool I_PauseKey()
{
  bool const res = s_pause_key;

  s_pause_key = false;

  return res;
}

int I_GetDirectionKey()
{
  if (s_bot == s_top)
  {
    return -1;
  }

  return s_direction_keys[s_bot];
}

int I_PopDirectionKey()
{
  if (s_bot == s_top)
  {
    return -1;
  }

  int const key = s_direction_keys[s_bot++];

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
    case 27:  // Escape
      ZGE_LeaveGameLoop();
      break;
    case 112:  // p
      s_pause_key = true;
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

  if (s_top > DIRECTION_KEYA_SIZE - 1)
  {
    s_top = 0;
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
      case GLFW_KEY_ESCAPE:
        ZGE_LeaveGameLoop();
        break;
      case GLFW_KEY_P:
        s_pause_key = true;
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

    if (s_top > DIRECTION_KEYA_SIZE - 1)
    {
      s_top = 0;
    }
  }
}

#endif

#if defined(USE_SDL2)

void ProcessKeyEvent(SDL_Event const* const e)
{
  if (e->type == SDL_KEYDOWN)
  {
    if (e->key.keysym.scancode == SDL_SCANCODE_ESCAPE)
    {
      ZGE_LeaveGameLoop();
    }
    else if (e->key.keysym.scancode == SDL_SCANCODE_P)
    {
      s_pause_key = true;
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

    if (s_top > DIRECTION_KEYA_SIZE - 1)
    {
      s_top = 0;
    }
  }
}

#endif
