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

  g_config.diceCount = DEFAULT_DICE_COUNT;
  g_config.shuffleFrequency = DEFAULT_SHUFFLE_FREQUENCY_HZ;

  if (argc > 1)
  {
    char* end;
    long diceCount = strtol(argv[1], &end, 10);

    if (diceCount != 0 && *end == '\0')
    {
      if (diceCount < MIN_DICE_COUNT)
      {
        diceCount = MIN_DICE_COUNT;
      }
      else if (diceCount > MAX_DICE_COUNT)
      {
        diceCount = MAX_DICE_COUNT;
      }

      g_config.diceCount = (int) diceCount;
    }
  }

  if (!ZGE_CreateWindow(g_config.diceCount * DICE_SIZE_PIXELS, DICE_SIZE_PIXELS, "Videostop"))
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
