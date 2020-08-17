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

  if (!zgeCreateWindow(g_config.dice_count * DICE_SIZE_PIXELS, DICE_SIZE_PIXELS, "Videostop"))
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
