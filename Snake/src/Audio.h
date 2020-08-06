#ifndef AUDIO_H
#define AUDIO_H

enum Sound
{
  Sound_Turn1 = 0,
  Sound_Turn2,
  Sound_Turn3,
  Sound_Turn4,
  Sound_Food,
  Sound_Wall,
  Sound_Body,
  Sound_Success,

  Sound_Count
};

void A_Start();
void A_Restart();
void A_Stop();

void A_PlaySound(enum Sound sound);

#endif  // AUDIO_H
