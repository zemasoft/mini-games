#include <stdbool.h>  // bool, false, true
#include <stddef.h>   // NULL, size_t
#include <stdlib.h>   // EXIT_FAILURE, EXIT_SUCCESS, atexit, strtol

#include "zge/core.h"
#include "zge/memory.h"

#include <whereami.h>

#include "Audio.h"
#include "Config.h"
#include "Graphics.h"
#include "Input.h"
#include "Logic.h"

char* g_executablePath;

static bool Initialize(int argc, char** argv);
static void Start();
static void Update(unsigned elapsed);
static void Stop();
static void Terminate();

int main(int const argc, char** const argv)
{
  if (!Initialize(argc, argv))
  {
    return EXIT_FAILURE;
  }

  atexit(Terminate);

  Start();
  Stop();

  return EXIT_SUCCESS;
}

bool Initialize(int argc, char** const argv)
{
  if (!ZGE_Initialize(&argc, argv))
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

  if (!ZGE_CreateWindow((int) g_config.size.x * PIECE_SIZE_PIXELS,
                        (int) g_config.size.y * PIECE_SIZE_PIXELS, "Sliding Puzzle"))
  {
    ZGE_Terminate();
    return false;
  }

  int const bufferLength = wai_getExecutablePath(NULL, 0, NULL);
  if (bufferLength >= 0)
  {
    g_executablePath = (char*) ZGE_Allocate((size_t)(bufferLength + 1));

    int pathLength;
    wai_getExecutablePath(g_executablePath, bufferLength, &pathLength);
    g_executablePath[pathLength] = '\0';
  }

  return true;
}

void Start()
{
  I_Start();
  L_Start();
  A_Start();
  G_Start();

  ZGE_SetResizeCallback(G_Resize);
  ZGE_SetUpdateCallback(Update);

  ZGE_EnterGameLoop();
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
  ZGE_SetResizeCallback(NULL);
  ZGE_SetUpdateCallback(NULL);

  I_Stop();
  L_Stop();
  A_Stop();
  G_Stop();
}

void Terminate()
{
  ZGE_FreeIfAllocated(g_executablePath);

  ZGE_DestroyWindow();

  ZGE_Terminate();
}
