// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#define MIN_X_SIZE 8
#define MAX_X_SIZE 32
#define DEFAULT_X_SIZE 16

#define MIN_Y_SIZE 8
#define MAX_Y_SIZE 32
#define DEFAULT_Y_SIZE 16

#define DIRECTION_KEYS_SIZE 4

#define FIELD_SIZE_PIXELS 25

#define FIELD_SIZE 1.0f
#define FOOD_RADIUS 0.3f * FIELD_SIZE

#define LIGHT_FIELD_COLOR 0.5f, 0.5f, 0.5f
#define DARK_FIELD_COLOR 0.25f, 0.25f, 0.25f

#define FOOD_COLOR 0.5f, 0.0f, 0.0f

#define SNAKE_COLOR 0.0f, 0.5f, 0.0f

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
