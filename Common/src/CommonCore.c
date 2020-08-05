#include "CommonCore.h"

#include <assert.h>   // assert
#include <stdbool.h>  // false, true
#include <stddef.h>   // size_t

#if defined(USE_FREEALUT)
#include <AL/alut.h>
#endif

#if defined(USE_FREEGLUT) || defined(USE_FREEGLUT_FOR_TEXT)
#include <GL/freeglut.h>
#endif

#if defined(USE_GLFW)
#include <GLFW/glfw3.h>
#endif

#if defined(USE_SDL2)
#include <SDL2/SDL.h>
#endif

static bool s_initialized;

static void (*s_terminate_fncs[4])(void);
static size_t s_terminate_fnc_count;

static void AtTerminate(void (*fnc)(void));
static void Terminate();

#if defined(USE_FREEALUT)
static void alutExitWrapper();
#endif

bool CC_Init(int argc, char** argv)
{
  if (s_initialized)
  {
    return true;
  }

#if defined(USE_FREEGLUT) || defined(USE_FREEGLUT_FOR_TEXT)
  glutInit(&argc, argv);

  AtTerminate(glutExit);
#endif

#if defined(USE_GLFW)
  if (!glfwInit())
  {
    Terminate();
    return false;
  }

  AtTerminate(glfwTerminate);
#endif

#if defined(USE_SDL2)
  if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0)
  {
    Terminate();
    return false;
  }

  AtTerminate(SDL_Quit);
#endif

#if defined(USE_FREEALUT)
  if (!alutInit(&argc, argv))
  {
    Terminate();
    return false;
  }

  AtTerminate(alutExitWrapper);
#endif

  s_initialized = true;

  return true;
}

void CC_Terminate()
{
  if (!s_initialized)
  {
    return;
  }

  Terminate();

  s_initialized = false;
}

unsigned CC_GetElapsedTime()
{
#if defined(USE_FREEGLUT)
  return (unsigned) glutGet(GLUT_ELAPSED_TIME);
#endif

#if defined(USE_GLFW)
  return (unsigned) (glfwGetTime() * 1000.0);
#endif

#if defined(USE_SDL2)
  return SDL_GetTicks();
#endif
}

void AtTerminate(void (*fnc)(void))
{
  assert(s_terminate_fnc_count < 4);

  s_terminate_fncs[s_terminate_fnc_count++] = fnc;
}

void Terminate()
{
  while (s_terminate_fnc_count > 0)
  {
    s_terminate_fncs[--s_terminate_fnc_count]();
  }
}

#if defined(USE_FREEALUT)

void alutExitWrapper()
{
  alutExit();
}

#endif
