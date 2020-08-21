#include "zge/audio.h"

#if defined(USE_FREEALUT_FOR_AUDIO)
#include <AL/alut.h>
#endif

static int s_nextSoundId;

#if defined(USE_FREEALUT_FOR_AUDIO)
static ALuint s_soundSourceId[256];
#endif

int ZGE_LoadSound(char const* const fileName)
{
#if !defined(USE_FREEALUT_FOR_AUDIO)
  (void) fileName;
#endif

#if defined(USE_FREEALUT_FOR_AUDIO)
  ALuint const buffer = alutCreateBufferFromFile(fileName);
  if (buffer != AL_NONE)
  {
    ALuint sourceId;
    alGenSources(1, &sourceId);
    alSourcei(sourceId, AL_BUFFER, (ALint) buffer);

    int soundId = s_nextSoundId++;

    s_soundSourceId[soundId] = sourceId;

    return soundId;
  }
#endif

  return -1;
}

void ZGE_PlaySound(int const soundId)
{
  if (soundId < 0 || soundId >= s_nextSoundId)
  {
    return;
  }

#if defined(USE_FREEALUT_FOR_AUDIO)
  alSourcePlay(s_soundSourceId[soundId]);
#endif
}
