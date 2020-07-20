#ifndef SOUND_H
#define SOUND_H

enum Sound
{
  Sound_Start = 0,
  Sound_Move,
  Sound_CannotMove,
  Sound_Success,

  Sound_Count
};

void S_Start();
void S_Restart();
void S_Stop();

void S_PlaySound(enum Sound sound);

#endif  // SOUND_H
