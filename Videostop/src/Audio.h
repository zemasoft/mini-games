#ifndef AUDIO_H
#define AUDIO_H

enum Sound
{
  Sound_Start = 0,
  Sound_Shuffle,
  Sound_Success,
  Sound_Fail,

  Sound_Count
};

void A_Start();
void A_Restart();
void A_Stop();

void A_PlaySound(enum Sound sound);

#endif  // AUDIO_H
