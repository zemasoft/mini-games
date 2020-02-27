// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <stdbool.h>  // bool, false, true
#include <stddef.h>   // size_t
#include <stdlib.h>   // EXIT_FAILURE, EXIT_SUCCESS, strtol

#if defined(USE_FREEALUT)
#include <AL/alut.h>
#endif

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
#include "Graphics.h"
#include "Input.h"
#include "Logic.h"
#include "Sound.h"

#if defined(USE_GLFW)
GLFWwindow* g_window;
#endif

#if defined(USE_SDL2)
SDL_Window* g_window;
bool g_quit;
#endif

static bool Init(int argc, char** argv);
static void Start();
static void Update();
static void Stop();
static void Terminate();

#if defined(USE_GLFW)
static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
#endif

#if defined(USE_SDL2)
static int WindowResizedEventWatcher(void* data, SDL_Event* event);
#endif

int main(int argc, char** argv)
{
  int exit_code = EXIT_SUCCESS;

#if defined(USE_FREEGLUT)
  glutInit(&argc, argv);
#endif

#if defined(USE_GLFW)
  if (!glfwInit())
  {
    exit_code = EXIT_FAILURE;
    goto err1;
  }
#endif

#if defined(USE_SDL2)
  if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0)
  {
    exit_code = EXIT_FAILURE;
    goto err2;
  }
#endif

#if defined(USE_FREEALUT)
  if (!alutInit(&argc, argv))
  {
    exit_code = EXIT_FAILURE;
    goto err3;
  }
#endif

  if (!Init(argc, argv))
  {
    exit_code = EXIT_FAILURE;
    goto err4;
  }

  Start();
  Stop();

  Terminate();

err4:

#if defined(USE_FREEALUT)
  alutExit();

err3:
#endif

#if defined(USE_SDL2)
  SDL_Quit();

err2:
#endif

#if defined(USE_GLFW)
  glfwTerminate();

err1:
#endif

#if defined(USE_FREEGLUT)
  glutExit();
#endif

  return exit_code;
}

bool Init(int argc, char** argv)
{
  g_config.size.x = DEFAULT_SIZE;
  g_config.size.y = DEFAULT_SIZE;

  if (argc > 1)
  {
    char* end;
    long y_size = strtol(argv[1], &end, 10);

    if (y_size != 0 && *end == '\0')
    {
      if (y_size < MIN_SIZE)
      {
        y_size = MIN_SIZE;
      }
      else if (y_size > MAX_SIZE)
      {
        y_size = MAX_SIZE;
      }

      g_config.size.x = (size_t) y_size;
      g_config.size.y = (size_t) y_size;
    }
  }

  if (argc > 2)
  {
    char* end;
    long x_size = strtol(argv[2], &end, 10);

    if (x_size != 0 && *end == '\0')
    {
      if (x_size < MIN_SIZE)
      {
        x_size = MIN_SIZE;
      }
      else if (x_size > MAX_SIZE)
      {
        x_size = MAX_SIZE;
      }

      g_config.size.x = (size_t) x_size;
    }
  }

#if defined(USE_FREEGLUT)
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
  glutInitWindowSize((int) g_config.size.x * PIECE_SIZE_PIXELS,
                     (int) g_config.size.y * PIECE_SIZE_PIXELS);
  glutCreateWindow("Sliding Puzzle");
#endif

#if defined(USE_GLFW)
  g_window =
      glfwCreateWindow((int) g_config.size.x * PIECE_SIZE_PIXELS,
                       (int) g_config.size.y * PIECE_SIZE_PIXELS, "Sliding Puzzle", NULL, NULL);
  if (g_window == NULL)
  {
    return false;
  }

  glfwSetFramebufferSizeCallback(g_window, FramebufferSizeCallback);

  glfwShowWindow(g_window);
  glfwMakeContextCurrent(g_window);
#endif

#if defined(USE_SDL2)
  g_window = SDL_CreateWindow("Sliding Puzzle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              (int) g_config.size.x * PIECE_SIZE_PIXELS,
                              (int) g_config.size.y * PIECE_SIZE_PIXELS,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (g_window == NULL)
  {
    return false;
  }

  SDL_AddEventWatch(WindowResizedEventWatcher, g_window);
#endif

  return true;
}

void Start()
{
  I_Start();
  L_Start();
  S_Start();
  G_Start();

#if defined(USE_FREEGLUT)
  glutDisplayFunc(&G_Update);
  glutIdleFunc(&Update);

  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  glutMainLoop();
#endif

#if defined(USE_GLFW)
  while (!glfwWindowShouldClose(g_window))
  {
    Update();
  }
#endif

#if defined(USE_SDL2)
  while (!g_quit)
  {
    Update();
  }
#endif
}

void Update()
{
  I_Update();
  L_Update();
  G_Update();
}

void Stop()
{
  I_Stop();
  L_Stop();
  S_Stop();
  G_Stop();

#if defined(USE_FREEGLUT)
  glutDisplayFunc(NULL);
  glutIdleFunc(NULL);
#endif
}

void Terminate()
{
#if defined(USE_GLFW)
  glfwDestroyWindow(g_window);
#endif

#if defined(USE_SDL2)
  SDL_DestroyWindow(g_window);
#endif
}

#if defined(USE_GLFW)

void FramebufferSizeCallback(GLFWwindow* window, int const width, int const height)
{
  (void) window;

  G_Resize(width, height);
}

#endif

#if defined(USE_SDL2)

int WindowResizedEventWatcher(void* data, SDL_Event* event)
{
  (void) data;

  if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED)
  {
    int width;
    int height;
    SDL_GL_GetDrawableSize(g_window, &width, &height);

    G_Resize(width, height);
  }

  return 0;
}

#endif
