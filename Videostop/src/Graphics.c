#include "Graphics.h"

#include <assert.h>   // assert
#include <math.h>     // M_PI, cos, sin
#include <stdbool.h>  // false
#include <stdio.h>    // snprintf

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

#if defined(USE_FREEGLUT) || defined(USE_FREEGLUT_FOR_TEXT)
#include <GL/freeglut.h>
#endif

#if defined(USE_GLFW)
#include <GLFW/glfw3.h>
#endif

#if defined(USE_SDL2)
#include <GL/gl.h>
#include <SDL2/SDL.h>
#endif

#include "CommonCore.h"

#include "Config.h"
#include "World.h"

static int s_init_window_width;
static int s_init_window_height;
static float s_init_width;
static float s_init_height;

static struct
{
  float x;
  float y;
} s_scale;

#if defined(USE_FREEGLUT_FOR_TEXT)
static struct
{
  float x;
  float y;
} s_string_scale;

static struct
{
  struct
  {
    float height;
  } string;
} s_statusBar;
#endif

static void RecountStatusBarString();
static void DrawDices();
static void DrawStatusBar();

static void DrawDice(int i);
static void DrawDots(int value);
static void DrawDot1();
static void DrawDot2();
static void DrawDot3();
static void DrawDot4();
static void DrawDot5();
static void DrawDot6();
static void DrawDot7();
static void DrawDot();

static float GetLeft();
static float GetRight();
static float GetBottom();
static float GetTop();

void G_Start()
{
  s_scale.x = 1.0f;
  s_scale.y = 1.0f;

  s_init_window_width = CC_GetWindowWidth();
  s_init_window_height = CC_GetWindowHeight();
  s_init_width = GetRight() - GetLeft();
  s_init_height = GetTop() - GetBottom();

#if defined(USE_FREEGLUT_FOR_TEXT)
  s_string_scale.x = s_init_width / (float) s_init_window_width;
  s_string_scale.y = s_init_height / (float) s_init_window_height;
#endif

  glEnable(GL_MULTISAMPLE);
  glDisable(GL_DEPTH_TEST);

  G_Restart();
}

void G_Restart()
{
  s_scale.x = (float) s_init_window_width / (float) CC_GetWindowWidth() * (GetRight() - GetLeft()) /
              s_init_width;
  s_scale.y = (float) s_init_window_height / (float) CC_GetWindowHeight() *
              (GetTop() - GetBottom()) / s_init_height;

  RecountStatusBarString();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(GetLeft(), GetRight(), GetBottom(), GetTop(), -1.0f, 1.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void G_Update()
{
  glClear(GL_COLOR_BUFFER_BIT);

  DrawDices();
  DrawStatusBar();

  CC_SwapBuffers();
}

void G_Stop()
{
}

void G_Resize(int const width, int const height)
{
  glViewport(0, 0, width, height);

  G_Restart();
}

void RecountStatusBarString()
{
#if defined(USE_FREEGLUT_FOR_TEXT)
  s_statusBar.string.height = (float) glutStrokeHeight(TEXT_FONT) * TEXT_SIZE * s_string_scale.y;
#endif
}

void DrawDices()
{
  glPushMatrix();

  for (int i = 0; i < g_world.dice_count; ++i)
  {
    DrawDice(i);

    glTranslatef(DICE_SIZE, 0.0f, 0.0f);
  }

  glPopMatrix();
}

void DrawStatusBar()
{
  glPushMatrix();

  float left = GetLeft();
  float top = -MARGIN / 2.0f;
  float right = GetRight();
  float bottom = GetBottom();

  switch (g_world.state)
  {
    case WorldState_Setup:
      glColor3f(STATUSBAR_SETUP_COLOR);
      break;
    case WorldState_Idle:
      glColor3f(STATUSBAR_IDLE_COLOR);
      break;
    case WorldState_Success:
      glColor3f(STATUSBAR_SUCCESS_COLOR);
      break;
    case WorldState_Fail:
      glColor3f(STATUSBAR_FAIL_COLOR);
      break;
  }

  // clang-format off
  glBegin(GL_POLYGON);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
  glEnd();
  // clang-format on

#if defined(USE_FREEGLUT_FOR_TEXT)
  glTranslatef(
      GetLeft() + MARGIN * s_scale.x,
      GetBottom() + (STATUSBAR_SIZE - s_statusBar.string.height * 0.75f) / 2.0f * s_scale.y, 0.0f);
  glScalef(TEXT_SIZE / 100.0f * s_scale.x, TEXT_SIZE / 100.0f * s_scale.y, 1.0f);
  glLineWidth(1.2f);

  if (g_world.state == WorldState_Setup)
  {
    glColor3f(STATUSBAR_DEFAULT_TEXT_COLOR);

    switch (g_world.statusBar.state)
    {
      case StatusBarState_Show:
        glutStrokeString(TEXT_FONT, (unsigned char*) "Press CONTROL to start!");
        break;
      case StatusBarState_Hide:
        break;
      case StatusBarState_Count:
        break;
    }
  }
  else
  {
    {
      char buf1[10];
      snprintf(buf1, sizeof(buf1), "%d", g_world.successful_attempts);

      char buf2[10];
      snprintf(buf2, sizeof(buf2), "%d", g_world.failed_attempts);

      glColor3f(STATUSBAR_DEFAULT_TEXT_COLOR);
      glutStrokeString(TEXT_FONT, (unsigned char*) "Attempts: ");
      glColor3f(STATUSBAR_SUCCESS_TEXT_COLOR);
      glutStrokeString(TEXT_FONT, (unsigned char*) &buf1[0]);
      glColor3f(STATUSBAR_DEFAULT_TEXT_COLOR);
      glutStrokeString(TEXT_FONT, (unsigned char*) " / ");
      glColor3f(STATUSBAR_FAIL_TEXT_COLOR);
      glutStrokeString(TEXT_FONT, (unsigned char*) &buf2[0]);
    }

    {
      char buf[20];
      snprintf(buf, sizeof(buf), " Score: %d", g_world.score);

      glColor3f(STATUSBAR_DEFAULT_TEXT_COLOR);
      glutStrokeString(TEXT_FONT, (unsigned char*) &buf[0]);
    }
  }
#endif

  glPopMatrix();
}

void DrawDice(int const i)
{
  float left = 0.0f;
  float top = DICE_SIZE;
  float right = DICE_SIZE;
  float bottom = 0.0f;

  switch (g_world.dices[i].state)
  {
    case DiceState_Setup:
      glColor3f(BORDER_SETUP_COLOR);
      break;
    case DiceState_Idle:
      glColor3f(BORDER_IDLE_COLOR);
      break;
    case DiceState_Success:
      glColor3f(BORDER_SUCCESS_COLOR);
      break;
    case DiceState_Fail:
      glColor3f(BORDER_FAIL_COLOR);
      break;
  }

  left += MARGIN;
  top -= MARGIN;
  right -= MARGIN;
  bottom += MARGIN;

  // clang-format off
  glBegin(GL_POLYGON);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
  glEnd();
  // clang-format on

  switch (g_world.dices[i].state)
  {
    case DiceState_Setup:
      glColor3f(DICE_SETUP_COLOR);
      break;
    case DiceState_Idle:
      glColor3f(DICE_IDLE_COLOR);
      break;
    case DiceState_Success:
      glColor3f(DICE_SUCCESS_COLOR);
      break;
    case DiceState_Fail:
      glColor3f(DICE_FAIL_COLOR);
      break;
  }

  left += BORDER;
  top -= BORDER;
  right -= BORDER;
  bottom += BORDER;

  // clang-format off
  glBegin(GL_POLYGON);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
  glEnd();
  // clang-format on

  switch (g_world.dices[i].state)
  {
    case DiceState_Setup:
      glColor3f(DOT_SETUP_COLOR);
      break;
    case DiceState_Idle:
      glColor3f(DOT_IDLE_COLOR);
      break;
    case DiceState_Success:
      glColor3f(DOT_SUCCESS_COLOR);
      break;
    case DiceState_Fail:
      glColor3f(DOT_FAIL_COLOR);
      break;
  }

  DrawDots(g_world.dices[i].value);
}

void DrawDots(int const value)
{
  switch (value)
  {
    case 1:
      DrawDot4();
      break;

    case 2:
      DrawDot3();
      DrawDot5();
      break;

    case 3:
      DrawDot3();
      DrawDot4();
      DrawDot5();
      break;

    case 4:
      DrawDot1();
      DrawDot3();
      DrawDot5();
      DrawDot7();
      break;

    case 5:
      DrawDot1();
      DrawDot3();
      DrawDot4();
      DrawDot5();
      DrawDot7();
      break;

    case 6:
      DrawDot1();
      DrawDot2();
      DrawDot3();
      DrawDot5();
      DrawDot6();
      DrawDot7();
      break;

    default:
      assert(false);
  }
}

void DrawDot1()
{
  glPushMatrix();
  glTranslatef(0.3f * DICE_SIZE, 0.7f * DICE_SIZE, 0.0f);

  DrawDot();

  glPopMatrix();
}

void DrawDot2()
{
  glPushMatrix();
  glTranslatef(0.3f * DICE_SIZE, 0.5f * DICE_SIZE, 0.0f);

  DrawDot();

  glPopMatrix();
}

void DrawDot3()
{
  glPushMatrix();
  glTranslatef(0.3f * DICE_SIZE, 0.3f * DICE_SIZE, 0.0f);

  DrawDot();

  glPopMatrix();
}

void DrawDot4()
{
  glPushMatrix();
  glTranslatef(0.5f * DICE_SIZE, 0.5f * DICE_SIZE, 0.0f);

  DrawDot();

  glPopMatrix();
}

void DrawDot5()
{
  glPushMatrix();
  glTranslatef(0.7f * DICE_SIZE, 0.7f * DICE_SIZE, 0.0f);

  DrawDot();

  glPopMatrix();
}

void DrawDot6()
{
  glPushMatrix();
  glTranslatef(0.7f * DICE_SIZE, 0.5f * DICE_SIZE, 0.0f);

  DrawDot();

  glPopMatrix();
}

void DrawDot7()
{
  glPushMatrix();
  glTranslatef(0.7f * DICE_SIZE, 0.3f * DICE_SIZE, 0.0f);

  DrawDot();

  glPopMatrix();
}

void DrawDot()
{
  glBegin(GL_POLYGON);

  for (int i = 0; i < 360; i += 15)
  {
    glVertex2f(DOT_RADIUS * cosf((float) (i * M_PI) / 180.0f),
               DOT_RADIUS * sinf((float) (i * M_PI) / 180.0f));
  }

  glEnd();
}

float GetLeft()
{
  return -MARGIN / 2.0f;
}

float GetRight()
{
  return (float) g_world.dice_count * DICE_SIZE + MARGIN / 2.0f;
}

float GetBottom()
{
  return -MARGIN / 2.0f - STATUSBAR_SIZE * s_scale.y;
}

float GetTop()
{
  return DICE_SIZE + MARGIN / 2.0f;
}
