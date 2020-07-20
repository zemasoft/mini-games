#ifndef CONFIG_H
#define CONFIG_H

#define UPDATE_PERIOD_MS 10

#define MIN_DICE_COUNT 2
#define MAX_DICE_COUNT 5
#define DEFAULT_DICE_COUNT 3

#define MIN_SHUFFLE_FREQUENCY_HZ 0.5f
#define MAX_SHUFFLE_FREQUENCY_HZ 5.0f
#define DEFAULT_SHUFFLE_FREQUENCY_HZ 1.5f

#define SHUFFLE_FREQUENCY_STEP_HZ 0.5f
#define STOP_TIME_MS 1000
#define STATUSBAR_FREQUENCY_HZ 2.0f

#define DICE_SIZE_PIXELS 200

#define DICE_SIZE 1.0f
#define MARGIN 0.025f * DICE_SIZE
#define BORDER 0.05f * DICE_SIZE
#define DOT_RADIUS 0.06f * DICE_SIZE
#define STATUSBAR_SIZE 0.12f * DICE_SIZE
#define TEXT_SIZE 0.07f * DICE_SIZE
#define TEXT_FONT GLUT_STROKE_MONO_ROMAN

#define BORDER_SETUP_COLOR 1.0f, 1.0f, 1.0f
#define BORDER_IDLE_COLOR 1.0f, 1.0f, 1.0f
#define BORDER_SUCCESS_COLOR 0.0f, 0.5f, 0.0f
#define BORDER_FAIL_COLOR 0.5f, 0.0f, 0.0f

#define DICE_SETUP_COLOR 0.0f, 0.0f, 0.5f
#define DICE_IDLE_COLOR 0.0f, 0.0f, 0.5f
#define DICE_SUCCESS_COLOR 0.0f, 0.0f, 0.5f
#define DICE_FAIL_COLOR 0.0f, 0.0f, 0.5f

#define DOT_SETUP_COLOR 1.0f, 1.0f, 1.0f
#define DOT_IDLE_COLOR 1.0f, 1.0f, 1.0f
#define DOT_SUCCESS_COLOR 1.0f, 1.0f, 1.0f
#define DOT_FAIL_COLOR 1.0f, 1.0f, 1.0f

#define STATUSBAR_SETUP_COLOR 0.2f, 0.2f, 0.2f
#define STATUSBAR_IDLE_COLOR 0.2f, 0.2f, 0.2f
#define STATUSBAR_SUCCESS_COLOR 0.2f, 0.2f, 0.2f
#define STATUSBAR_FAIL_COLOR 0.2f, 0.2f, 0.2f

#define STATUSBAR_DEFAULT_TEXT_COLOR 1.0f, 1.0f, 1.0f
#define STATUSBAR_SUCCESS_TEXT_COLOR 0.0f, 0.5f, 0.0f
#define STATUSBAR_FAIL_TEXT_COLOR 0.5f, 0.0f, 0.0f

struct Config
{
  int dice_count;
  float shuffle_frequency;
};

extern struct Config g_config;

#endif  // CONFIG_H
