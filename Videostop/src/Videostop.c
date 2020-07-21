#include <stdbool.h>  // bool, false, true
#include <stdlib.h>   // EXIT_FAILURE, EXIT_SUCCESS, strtol

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

#if defined(USE_FREEGLUT) || defined(USE_FREEGLUT_FOR_TEXT)
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

#if defined(USE_FREEGLUT) || defined(USE_FREEGLUT_FOR_TEXT)
  glutExit();
#endif

  return exit_code;
}

bool Init(int argc, char** argv)
{
  g_config.dice_count = DEFAULT_DICE_COUNT;
  g_config.shuffle_frequency = DEFAULT_SHUFFLE_FREQUENCY_HZ;

  if (argc > 1)
  {
    char* end;
    long dice_count = strtol(argv[1], &end, 10);

    if (dice_count != 0 && *end == '\0')
    {
      if (dice_count < MIN_DICE_COUNT)
      {
        dice_count = MIN_DICE_COUNT;
      }
      else if (dice_count > MAX_DICE_COUNT)
      {
        dice_count = MAX_DICE_COUNT;
      }

      g_config.dice_count = (int) dice_count;
    }
  }

#if defined(USE_FREEGLUT)
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
  glutInitWindowSize(g_config.dice_count * DICE_SIZE_PIXELS, DICE_SIZE_PIXELS);
  glutCreateWindow("Videostop");
#endif

#if defined(USE_GLFW)
  glfwWindowHint(GLFW_SAMPLES, 4);

  g_window = glfwCreateWindow(g_config.dice_count * DICE_SIZE_PIXELS, DICE_SIZE_PIXELS, "Videostop",
                              NULL, NULL);
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
  g_window = SDL_CreateWindow("Videostop", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              g_config.dice_count * DICE_SIZE_PIXELS, DICE_SIZE_PIXELS,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (g_window == NULL)
  {
    return false;
  }

  SDL_GL_CreateContext(g_window);
  SDL_GL_SetSwapInterval(1);

  SDL_AddEventWatch(WindowResizedEventWatcher, g_window);

  SDL_ShowWindow(g_window);
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
  glutReshapeFunc(&G_Resize);
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

#if defined(USE_FREEGLUT)
  unsigned const now = (unsigned) glutGet(GLUT_ELAPSED_TIME);
#endif

#if defined(USE_GLFW)
  unsigned const now = (unsigned) (glfwGetTime() * 1000.0);
#endif

#if defined(USE_SDL2)
  unsigned const now = SDL_GetTicks();
#endif

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
  S_Stop();
  G_Stop();

#if defined(USE_FREEGLUT)
  glutDisplayFunc(NULL);
  glutReshapeFunc(NULL);
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

void FramebufferSizeCallback(GLFWwindow* const window, int const width, int const height)
{
  (void) window;

  G_Resize(width, height);
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
    SDL_GL_GetDrawableSize(g_window, &width, &height);

    G_Resize(width, height);
  }

  return 0;
}

#endif
