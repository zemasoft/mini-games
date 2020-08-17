#include <stdbool.h>  // bool, false, true
#include <stddef.h>   // size_t
#include <stdlib.h>   // EXIT_FAILURE, EXIT_SUCCESS, free, malloc, strtol

#include "zge/zge.h"

#include <whereami.h>

#include "Audio.h"
#include "Config.h"
#include "Graphics.h"
#include "Input.h"
#include "Logic.h"

char* g_executable_path;

static bool Initialize(int argc, char** argv);
static void Start();
static void Update(unsigned elapsed);
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
  if (!zgeInitialize(&argc, argv))
  {
    return false;
  }

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

  if (!zgeCreateWindow((int) g_config.size.x * PIECE_SIZE_PIXELS,
                       (int) g_config.size.y * PIECE_SIZE_PIXELS, "Sliding Puzzle"))
  {
    zgeTerminate();
    return false;
  }

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

  zgeSetResizeCallback(G_Resize);
  zgeSetUpdateCallback(Update);

  zgeEnterMainLoop();
}

void Update(unsigned const elapsed)
{
  static float lag;

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
  zgeSetResizeCallback(NULL);
  zgeSetUpdateCallback(NULL);

  I_Stop();
  L_Stop();
  A_Stop();
  G_Stop();
}

void Terminate()
{
  if (g_executable_path != NULL)
  {
    free(g_executable_path);
  }

  zgeDestroyWindow();

  zgeTerminate();
}
