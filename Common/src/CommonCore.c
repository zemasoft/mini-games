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

#if defined(USE_GLFW)
GLFWwindow* g_window;
#endif

static bool s_initialized;

// Terminate

typedef void (*TerminateFnc)(void);

static TerminateFnc s_terminateFncs[4];
static size_t s_terminateFncCount;

static void AtTerminate(TerminateFnc terminateFnc);
static void Terminate();

// Terminate end

#if defined(USE_FREEALUT)
static void alutExitWrapper();
#endif

#if defined(USE_FREEGLUT)
static int s_window;
#endif

#if defined(USE_GLFW)
static CC_ResizeFnc s_resizeFnc;
static CC_UpdateFnc s_updateFnc;

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
#endif

#if defined(USE_SDL2)
static SDL_Window* s_window;
static CC_ResizeFnc s_resizeFnc;
static CC_UpdateFnc s_updateFnc;
static bool s_leaveMainLoop;

static int WindowResizedEventWatcher(void* data, SDL_Event* event);
#endif

bool CC_Initialize(int argc, char** argv)
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
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
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

bool CC_CreateWindow(int const width, int const height, char const* const title)
{
#if defined(USE_FREEGLUT)
  if (s_window != 0)
  {
    return false;
  }

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);  // TODO: verify
  glutInitWindowSize(width, height);

  s_window = glutCreateWindow(title);
  if (s_window == 0)
  {
    return false;
  }
#endif

#if defined(USE_GLFW)
  if (g_window != NULL)
  {
    return false;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);  // TODO: verify

  g_window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (g_window == NULL)
  {
    return false;
  }

  glfwMakeContextCurrent(g_window);
  glfwSwapInterval(1);

  glfwSetFramebufferSizeCallback(g_window, FramebufferSizeCallback);

  glfwShowWindow(g_window);
#endif

#if defined(USE_SDL2)
  if (s_window != NULL)
  {
    return false;
  }

  s_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width,
                              height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (s_window == NULL)
  {
    return false;
  }

  SDL_GL_CreateContext(s_window);
  SDL_GL_SetSwapInterval(1);

  SDL_AddEventWatch(WindowResizedEventWatcher, s_window);

  SDL_ShowWindow(s_window);
#endif

  return true;
}

void CC_DestroyWindow()
{
#if defined(USE_FREEGLUT)
  if (s_window != 0)
  {
    glutDestroyWindow(s_window);

    s_window = 0;
  }
#endif

#if defined(USE_GLFW)
  if (g_window != NULL)
  {
    glfwDestroyWindow(g_window);

    g_window = NULL;
  }
#endif

#if defined(USE_SDL2)
  if (s_window != NULL)
  {
    SDL_DestroyWindow(s_window);

    s_window = NULL;
  }
#endif
}

int CC_GetWindowWidth()
{
#if defined(USE_FREEGLUT)
  return glutGet(GLUT_WINDOW_WIDTH);
#endif

#if defined(USE_GLFW)
  int width;
  int height;
  glfwGetWindowSize(g_window, &width, &height);

  return width;
#endif

#if defined(USE_SDL2)
  int width;
  int height;
  SDL_GetWindowSize(s_window, &width, &height);

  return width;
#endif
}

int CC_GetWindowHeight()
{
#if defined(USE_FREEGLUT)
  return glutGet(GLUT_WINDOW_HEIGHT);
#endif

#if defined(USE_GLFW)
  int width;
  int height;
  glfwGetWindowSize(g_window, &width, &height);

  return height;
#endif

#if defined(USE_SDL2)
  int width;
  int height;
  SDL_GetWindowSize(s_window, &width, &height);

  return height;
#endif
}

void CC_SetWindowTitle(char const* const title)
{
#if defined(USE_FREEGLUT)
  glutSetWindowTitle(title);
#endif

#if defined(USE_GLFW)
  glfwSetWindowTitle(g_window, title);
#endif

#if defined(USE_SDL2)
  SDL_SetWindowTitle(s_window, title);
#endif
}

void CC_SetDisplayFnc(CC_DisplayFnc const displayFnc)
{
#if defined(USE_FREEGLUT)
  glutDisplayFunc(displayFnc);
#endif

#if defined(USE_GLFW)
  (void) displayFnc;
#endif

#if defined(USE_SDL2)
  (void) displayFnc;
#endif
}

void CC_SetResizeFnc(CC_ResizeFnc const resizeFnc)
{
#if defined(USE_FREEGLUT)
  glutReshapeFunc(resizeFnc);
#endif

#if defined(USE_GLFW)
  s_resizeFnc = resizeFnc;
#endif

#if defined(USE_SDL2)
  s_resizeFnc = resizeFnc;
#endif
}

void CC_SetUpdateFnc(CC_UpdateFnc const updateFnc)
{
#if defined(USE_FREEGLUT)
  glutIdleFunc(updateFnc);
#endif

#if defined(USE_GLFW)
  s_updateFnc = updateFnc;
#endif

#if defined(USE_SDL2)
  s_updateFnc = updateFnc;
#endif
}

void CC_EnterMainLoop()
{
#if defined(USE_FREEGLUT)
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  glutMainLoop();
#endif

#if defined(USE_GLFW)
  while (!glfwWindowShouldClose(g_window))
  {
    if (s_updateFnc != NULL)
    {
      s_updateFnc();
    }
  }
#endif

#if defined(USE_SDL2)
  while (!s_leaveMainLoop)
  {
    if (s_updateFnc != NULL)
    {
      s_updateFnc();
    }
  }
#endif
}

void CC_LeaveMainLoop()
{
#if defined(USE_FREEGLUT)
  glutLeaveMainLoop();
#endif

#if defined(USE_GLFW)
  glfwSetWindowShouldClose(g_window, GLFW_TRUE);
#endif

#if defined(USE_SDL2)
  s_leaveMainLoop = true;
#endif
}

void CC_SwapBuffers()
{
#if defined(USE_FREEGLUT)
  glutSwapBuffers();
#endif

#if defined(USE_GLFW)
  glfwSwapBuffers(g_window);
#endif

#if defined(USE_SDL2)
  SDL_GL_SwapWindow(s_window);
#endif
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

void AtTerminate(TerminateFnc const terminateFnc)
{
  assert(s_terminateFncCount < 4);

  s_terminateFncs[s_terminateFncCount++] = terminateFnc;
}

void Terminate()
{
  while (s_terminateFncCount > 0)
  {
    s_terminateFncs[--s_terminateFncCount]();
  }
}

#if defined(USE_FREEALUT)

void alutExitWrapper()
{
  alutExit();
}

#endif

#if defined(USE_GLFW)

void FramebufferSizeCallback(GLFWwindow* const window, int const width, int const height)
{
  (void) window;

  if (s_resizeFnc != NULL)
  {
    s_resizeFnc(width, height);
  }
}

#endif

#if defined(USE_SDL2)

int WindowResizedEventWatcher(void* const data, SDL_Event* const event)
{
  (void) data;

  if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED)
  {
    int width;
    int height;
    SDL_GL_GetDrawableSize(s_window, &width, &height);

    if (s_resizeFnc != NULL)
    {
      s_resizeFnc(width, height);
    }
  }

  return 0;
}

#endif