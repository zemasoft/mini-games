#include <stdbool.h>  // bool, false, true
#include <stddef.h>   // size_t
#include <stdlib.h>   // EXIT_FAILURE, EXIT_SUCCESS, free, malloc, strtol

#include "CommonCore.h"

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

#include <whereami.h>

#include "Audio.h"
#include "Config.h"
#include "Graphics.h"
#include "Input.h"
#include "Logic.h"

#if defined(USE_GLFW)
GLFWwindow* g_window;
#endif

#if defined(USE_SDL2)
SDL_Window* g_window;
bool g_quit;
#endif

char* g_executable_path;

static bool Initialize(int argc, char** argv);
static void Start();
static void Update();
static void Stop();
static void Terminate();

int main(int argc, char** argv)
{
  if (!Initialize(argc, argv))
  {
    return EXIT_FAILURE;
  }

  Start();
  Stop();

  Terminate();

  return EXIT_SUCCESS;
}

bool Initialize(int argc, char** argv)
{
  if (!CC_Initialize(argc, argv))
  {
    return false;
  }

  g_config.ground.size.x = DEFAULT_X_SIZE;
  g_config.ground.size.y = DEFAULT_Y_SIZE;

  if (argc > 1)
  {
    char* end;
    long y_size = strtol(argv[1], &end, 10);

    if (y_size != 0 && *end == '\0')
    {
      if (y_size < MIN_Y_SIZE)
      {
        y_size = MIN_Y_SIZE;
      }
      else if (y_size > MAX_Y_SIZE)
      {
        y_size = MAX_Y_SIZE;
      }

      g_config.ground.size.x = (int) y_size;
      g_config.ground.size.y = (int) y_size;
    }
  }

  if (argc > 2)
  {
    char* end;
    long x_size = strtol(argv[2], &end, 10);

    if (x_size != 0 && *end == '\0')
    {
      if (x_size < MIN_X_SIZE)
      {
        x_size = MIN_X_SIZE;
      }
      else if (x_size > MAX_X_SIZE)
      {
        x_size = MAX_X_SIZE;
      }

      g_config.ground.size.x = (int) x_size;
    }
  }

#if defined(USE_FREEGLUT)
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
  glutInitWindowSize((int) g_config.ground.size.x * FIELD_SIZE_PIXELS,
                     (int) g_config.ground.size.y * FIELD_SIZE_PIXELS);
  glutCreateWindow("Snake");
#endif

#if defined(USE_GLFW)
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  g_window =
      glfwCreateWindow((int) g_config.ground.size.x * FIELD_SIZE_PIXELS,
                       (int) g_config.ground.size.y * FIELD_SIZE_PIXELS, "Snake", NULL, NULL);
  if (g_window == NULL)
  {
    CC_Terminate();
    return false;
  }

  glfwMakeContextCurrent(g_window);
  glfwSwapInterval(1);

  glfwShowWindow(g_window);
#endif

#if defined(USE_SDL2)
  g_window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              (int) g_config.ground.size.x * FIELD_SIZE_PIXELS,
                              (int) g_config.ground.size.y * FIELD_SIZE_PIXELS, SDL_WINDOW_OPENGL);
  if (g_window == NULL)
  {
    CC_Terminate();
    return false;
  }

  SDL_GL_CreateContext(g_window);
  SDL_GL_SetSwapInterval(1);

  SDL_ShowWindow(g_window);
#endif

  int const buffer_length = wai_getExecutablePath(NULL, 0, NULL);
  if (buffer_length >= 0)
  {
    g_executable_path = (char*) malloc((size_t)(buffer_length + 1));

    int path_length;
    wai_getExecutablePath(g_executable_path, buffer_length, &path_length);
    g_executable_path[path_length] = '\0';
  }

  return true;
}

void Start()
{
  I_Start();
  L_Start();
  A_Start();
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
  static unsigned before;
  static float lag;

  unsigned const now = CC_GetElapsedTime();

  if (before == 0)
  {
    before = now;
  }

  unsigned const elapsed = now - before;
  before = now;
  lag += (float) elapsed;

  I_Update();

  while (lag >= UPDATE_PERIOD_MS)
  {
    L_Update();
    lag -= UPDATE_PERIOD_MS;
  }

  G_Update();
}

void Stop()
{
  I_Stop();
  L_Stop();
  A_Stop();
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

  if (g_executable_path != NULL)
  {
    free(g_executable_path);
  }

  CC_Terminate();
}
