#ifndef SOUND_H
#define SOUND_H

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

void S_Start();
void S_Restart();
void S_Stop();

void S_PlaySound(enum Sound sound);

#endif  // SOUND_H
