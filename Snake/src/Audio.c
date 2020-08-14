#include "Audio.h"

#include <string.h>  // strcat, strcpy

#include "CommonCore.h"

extern char* g_executable_path;

static char const* const s_soundFileNames[Sound_Count] = {"turn1.wav", "turn2.wav",  "turn3.wav",
                                                          "turn4.wav", "food.wav",   "wall.wav",
                                                          "body.wav",  "success.wav"};
static int s_soundIds[Sound_Count];

void A_Start()
{
  for (int sound = 0; sound < Sound_Count; ++sound)
  {
    char fileName[256];
    strcpy(fileName, g_executable_path);
#if defined(_WIN32)
    strcat(fileName, "\\assets\\");
#elif defined(__linux__)
    strcat(fileName, "/../assets/");
#endif
    strcat(fileName, s_soundFileNames[sound]);

    s_soundIds[sound] = CC_LoadSound(fileName);
  }
}

void A_Restart()
{
}

void A_Stop()
{
}

void A_PlaySound(enum Sound const sound)
{
  CC_PlaySound(s_soundIds[sound]);
}
