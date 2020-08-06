#include "Audio.h"

#include <string.h>  // strcat, strcpy

#if defined(USE_FREEALUT)
#include <AL/alut.h>
#endif

extern char* g_executable_path;

#if defined(USE_FREEALUT)
static char const* const s_sound_file_names[Sound_Count] = {"turn1.wav", "turn2.wav",  "turn3.wav",
                                                            "turn4.wav", "food.wav",   "wall.wav",
                                                            "body.wav",  "success.wav"};
#endif

#if defined(USE_FREEALUT)
static ALuint s_sound_sources[Sound_Count];
#endif

void A_Start()
{
#if defined(USE_FREEALUT)
  for (int i = 0; i < Sound_Count; ++i)
  {
    char file_name[256];
    strcpy(file_name, g_executable_path);
#if defined(_WIN32)
    strcat(file_name, "\\assets\\");
#elif defined(__linux__)
    strcat(file_name, "/../assets/");
#endif
    strcat(file_name, s_sound_file_names[i]);

    ALuint buffer = alutCreateBufferFromFile(file_name);
    if (buffer != AL_NONE)
    {
      alGenSources(1, &s_sound_sources[i]);
      alSourcei(s_sound_sources[i], AL_BUFFER, (ALint) buffer);
    }
  }
#endif
}

void A_Restart()
{
}

void A_Stop()
{
}

void A_PlaySound(enum Sound const sound)
{
#if !defined(USE_FREEALUT)
  (void) sound;
#endif

#if defined(USE_FREEALUT)
  alSourcePlay(s_sound_sources[sound]);
#endif
}
