// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#define MIN_X_SIZE 8
#define MAX_X_SIZE 32
#define DEFAULT_X_SIZE 17

#define MIN_Y_SIZE 8
#define MAX_Y_SIZE 32
#define DEFAULT_Y_SIZE 15

#define DIRECTION_KEYS_SIZE 4

#define FIELD_SIZE_PIXELS 32

#define FIELD_SIZE 1.0f
#define STATUSBAR_SIZE 2.0f * FIELD_SIZE
#define MARGIN_SIZE 1.0f * FIELD_SIZE
#define FOOD_RADIUS 0.3f * FIELD_SIZE

#define STATUSBAR_COLOR 74, 117, 44
#define MARGIN_COLOR 87, 138, 52
#define LIGHT_FIELD_COLOR 170, 215, 81
#define DARK_FIELD_COLOR 162, 209, 73

#define FOOD_COLOR 128, 0, 0

#define SNAKE_COLOR 78, 124, 246

struct GameConfig
{
  struct
  {
    int x;
    int y;
  } size;
};

extern struct GameConfig g_game_config;

#endif  // GAME_CONFIG_H
