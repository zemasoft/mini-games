#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>  // size_t

#define UPDATE_PERIOD_MS (1000.0f / 240.0f)

#define MIN_SIZE 2
#define MAX_SIZE 10
#define DEFAULT_SIZE 4

#define STRING_SIZE 3  // floor(log10(MAX_SIZE^2 - 1)) + 1 + 1

#define DIRECTION_KEYA_SIZE 4

#define MOVE_STEP (0.1f * 60.0f / 1000.0f * UPDATE_PERIOD_MS)
#define STATUSBAR_FREQUENCY_HZ 2.0f

#define PIECE_SIZE_PIXELS 100

#define PIECE_SIZE 1.0f
#define MARGIN 0.02f * PIECE_SIZE
#define BORDER 0.05f * PIECE_SIZE
#define VALUE_SIZE 0.30f * PIECE_SIZE
#define STATUSBAR_SIZE 0.24f * PIECE_SIZE
#define TEXT_SIZE 0.14f * PIECE_SIZE
#define TEXT_FONT GLUT_STROKE_MONO_ROMAN

#define BORDER_SETUP_COLOR 1.0f, 1.0f, 1.0f
#define BORDER_IDLE_COLOR 1.0f, 1.0f, 1.0f
#define BORDER_MOVING_COLOR 1.0f, 1.0f, 1.0f
#define BORDER_SUCCESS_COLOR 0.0f, 0.5f, 0.0f

#define PIECE_SETUP_COLOR 0.0f, 0.0f, 0.5f
#define PIECE_IDLE_COLOR 0.0f, 0.0f, 0.5f
#define PIECE_MOVING_COLOR 0.0f, 0.0f, 0.5f
#define PIECE_SUCCESS_COLOR 0.0f, 0.0f, 0.5f

#define VALUE_SETUP_COLOR 1.0f, 1.0f, 1.0f
#define VALUE_IDLE_COLOR 1.0f, 1.0f, 1.0f
#define VALUE_MOVING_COLOR 1.0f, 1.0f, 1.0f
#define VALUE_SUCCESS_COLOR 1.0f, 1.0f, 1.0f

#define STATUSBAR_SETUP_COLOR 0.2f, 0.2f, 0.2f
#define STATUSBAR_IDLE_COLOR 0.2f, 0.2f, 0.2f
#define STATUSBAR_MOVING_COLOR 0.2f, 0.2f, 0.2f
#define STATUSBAR_SUCCESS_COLOR 0.2f, 0.2f, 0.2f

#define STATUSBAR_TEXT_COLOR 1.0f, 1.0f, 1.0f

struct Config
{
  struct
  {
    size_t x;
    size_t y;
  } size;
};

extern struct Config g_config;

#endif  // CONFIG_H
