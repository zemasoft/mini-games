// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SOUND_H
#define SOUND_H

enum Sound
{
  Sound_Move = 0,

  Sound_Count
};

void S_Start();
void S_Stop();

void S_PlaySound(enum Sound sound);

#endif  // SOUND_H
