// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Graphics.h"

#include <math.h>   // M_PI, cos, sin
#include <stdio.h>  // snprintf

#include <GL/freeglut.h>

#include "GameState.h"

#define DICE_SIZE 1.0f
#define MARGIN 0.025f * DICE_SIZE
#define BORDER 0.05f * DICE_SIZE
#define DOT_RADIUS 0.06f * DICE_SIZE
#define STATUSBAR_SIZE 0.12f * DICE_SIZE
#define TEXT_SIZE 0.07f * DICE_SIZE

#define BORDER_DEFAULT_COLOR 1.0f, 1.0f, 1.0f
#define BORDER_SUCCESS_COLOR 0.0f, 0.5f, 0.0f
#define BORDER_FAIL_COLOR 0.5f, 0.0f, 0.0f

#define DICE_DEFAULT_COLOR 0.0f, 0.0f, 0.5f
#define DICE_SUCCESS_COLOR 0.0f, 0.0f, 0.5f
#define DICE_FAIL_COLOR 0.0f, 0.0f, 0.5f

#define DOT_DEFAULT_COLOR 1.0f, 1.0f, 1.0f
#define DOT_SUCCESS_COLOR 1.0f, 1.0f, 1.0f
#define DOT_FAIL_COLOR 1.0f, 1.0f, 1.0f

#define STATUSBAR_DEFAULT_COLOR 1.0f, 1.0f, 1.0f
#define STATUSBAR_SUCCESS_COLOR 0.0f, 0.5f, 0.0f
#define STATUSBAR_FAIL_COLOR 0.5f, 0.0f, 0.0f

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

static void DrawAttempts();
static void DrawScore();

void G_Start()
{
  glEnable(GL_MULTISAMPLE);

  glDisable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-MARGIN / 2.0f, (float) g_game_state.dice_count * DICE_SIZE + MARGIN / 2.0f,
          -MARGIN / 2.0f - STATUSBAR_SIZE, DICE_SIZE + MARGIN / 2.0f, -1.0f, 1.0f);

  glMatrixMode(GL_MODELVIEW);
}

void G_Update()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  DrawDices();
  DrawStatusBar();

  glutSwapBuffers();
}

void G_Stop()
{
}

void DrawDices()
{
  glPushMatrix();

  for (int i = 0; i < g_game_state.dice_count; ++i)
  {
    DrawDice(i);

    glTranslatef(DICE_SIZE, 0.0f, 0.0f);
  }

  glPopMatrix();
}

void DrawStatusBar()
{
  glPushMatrix();

  glTranslatef(MARGIN / 2.0f, -MARGIN / 2.0f - TEXT_SIZE, 0.0f);
  glScalef(TEXT_SIZE / 100.0f, TEXT_SIZE / 100.0f, 1.0f);
  glLineWidth(1.2f);

  DrawAttempts();
  DrawScore();

  glPopMatrix();
}

void DrawDice(int i)
{
  float left = 0.0f;
  float top = DICE_SIZE;
  float right = DICE_SIZE;
  float bottom = 0.0f;

  switch (g_game_state.dices[i].state)
  {
    case State_Shuffle:
      glColor3f(BORDER_DEFAULT_COLOR);
      break;
    case State_Success:
      glColor3f(BORDER_SUCCESS_COLOR);
      break;
    case State_Fail:
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

  switch (g_game_state.dices[i].state)
  {
    case State_Shuffle:
      glColor3f(DICE_DEFAULT_COLOR);
      break;
    case State_Success:
      glColor3f(DICE_SUCCESS_COLOR);
      break;
    case State_Fail:
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

  switch (g_game_state.dices[i].state)
  {
    case State_Shuffle:
      glColor3f(DOT_DEFAULT_COLOR);
      break;
    case State_Success:
      glColor3f(DOT_SUCCESS_COLOR);
      break;
    case State_Fail:
      glColor3f(DOT_FAIL_COLOR);
      break;
  }

  DrawDots(g_game_state.dices[i].value);
}

void DrawDots(int value)
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

void DrawAttempts()
{
  char buf1[10];
  snprintf(buf1, sizeof(buf1), "%d", g_game_state.successful_attempts);

  char buf2[10];
  snprintf(buf2, sizeof(buf2), "%d", g_game_state.failed_attempts);

  glColor3f(STATUSBAR_DEFAULT_COLOR);
  glutStrokeString(GLUT_STROKE_MONO_ROMAN, (unsigned char*) "Attempts: ");
  glColor3f(STATUSBAR_SUCCESS_COLOR);
  glutStrokeString(GLUT_STROKE_MONO_ROMAN, (unsigned char*) &buf1[0]);
  glColor3f(STATUSBAR_DEFAULT_COLOR);
  glutStrokeString(GLUT_STROKE_MONO_ROMAN, (unsigned char*) " / ");
  glColor3f(STATUSBAR_FAIL_COLOR);
  glutStrokeString(GLUT_STROKE_MONO_ROMAN, (unsigned char*) &buf2[0]);
}

void DrawScore()
{
  char buf[20];
  snprintf(buf, sizeof(buf), " Score: %d", g_game_state.score);

  glColor3f(STATUSBAR_DEFAULT_COLOR);
  glutStrokeString(GLUT_STROKE_MONO_ROMAN, (unsigned char*) &buf[0]);
}
