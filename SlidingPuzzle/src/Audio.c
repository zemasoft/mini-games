#include "Audio.h"

#include <string.h>  // strcat, strcpy

#include "zge/audio.h"

extern char* g_executablePath;

static char const* const s_soundFileNames[Sound_Count] = {"start.wav", "move.wav",
                                                          "cannot-move.wav", "success.wav"};
static int s_soundIds[Sound_Count];

void A_Start()
{
  for (int sound = 0; sound < Sound_Count; ++sound)
  {
    char fileName[256];
    strcpy(fileName, g_executablePath);
#if defined(_WIN32)
    strcat(fileName, "\\assets\\");
#elif defined(__linux__)
    strcat(fileName, "/../assets/");
#endif
    strcat(fileName, s_soundFileNames[sound]);

    s_soundIds[sound] = ZGE_LoadSound(fileName);
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
  ZGE_PlaySound(s_soundIds[sound]);
}
