// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Sound.h"

#include <AL/alut.h>

static const char* const s_sound_file_names[Sound_Count] = {"start.wav", "move.wav",
                                                            "cannot-move.wav", "success.wav"};

static ALuint s_sound_sources[Sound_Count];

void S_Start()
{
  for (int i = 0; i < Sound_Count; ++i)
  {
    ALuint buffer = alutCreateBufferFromFile(s_sound_file_names[i]);
    if (buffer != AL_NONE)
    {
      alGenSources(1, &s_sound_sources[i]);
      alSourcei(s_sound_sources[i], AL_BUFFER, (ALint) buffer);
    }
  }
}

void S_Restart()
{
}

void S_Stop()
{
}

void S_PlaySound(enum Sound const sound)
{
  alSourcePlay(s_sound_sources[sound]);
}
