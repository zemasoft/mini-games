#include "Sound.h"

#if defined(USE_FREEALUT)
#include <AL/alut.h>
#endif

#if defined(USE_FREEALUT)
static char const* const s_sound_file_names[Sound_Count] = {"turn1.wav", "turn2.wav",  "turn3.wav",
                                                            "turn4.wav", "food.wav",   "wall.wav",
                                                            "body.wav",  "success.wav"};
#endif

#if defined(USE_FREEALUT)
static ALuint s_sound_sources[Sound_Count];
#endif

void S_Start()
{
#if defined(USE_FREEALUT)
  for (int i = 0; i < Sound_Count; ++i)
  {
    ALuint buffer = alutCreateBufferFromFile(s_sound_file_names[i]);
    if (buffer != AL_NONE)
    {
      alGenSources(1, &s_sound_sources[i]);
      alSourcei(s_sound_sources[i], AL_BUFFER, (ALint) buffer);
    }
  }
#endif
}

void S_Restart()
{
}

void S_Stop()
{
}

void S_PlaySound(enum Sound const sound)
{
#if !defined(USE_FREEALUT)
  (void) sound;
#endif

#if defined(USE_FREEALUT)
  alSourcePlay(s_sound_sources[sound]);
#endif
}
