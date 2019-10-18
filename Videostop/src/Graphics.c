// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Graphics.h"

#include <assert.h>   // assert
#include <math.h>     // M_PI, cos, sin
#include <stdbool.h>  // false
#include <stdio.h>    // snprintf

#include <GL/freeglut.h>

#include "GameConfig.h"
#include "GameState.h"

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

  G_Restart();
}

void G_Restart()
{
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

  if (g_game_state.state == State_Setup)
  {
    glColor3f(STATUSBAR_DEFAULT_COLOR);

    switch (g_game_state.statusbar_state)
    {
      case StatusBar_State_Show:
        glutStrokeString(TEXT_FONT, (unsigned char*) "Press CONTROL to start!");
        break;
      case StatusBar_State_Hide:
        break;
      case StatusBar_State_Count:
        break;
    }
  }
  else
  {
    DrawAttempts();
    DrawScore();
  }

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
    case State_Setup:
      glColor3f(BORDER_SETUP_COLOR);
      break;
    case State_Idle:
      glColor3f(BORDER_IDLE_COLOR);
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
    case State_Setup:
      glColor3f(DICE_SETUP_COLOR);
      break;
    case State_Idle:
      glColor3f(DICE_IDLE_COLOR);
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
    case State_Setup:
      glColor3f(DOT_SETUP_COLOR);
      break;
    case State_Idle:
      glColor3f(DOT_IDLE_COLOR);
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

void DrawAttempts()
{
  char buf1[10];
  snprintf(buf1, sizeof(buf1), "%d", g_game_state.successful_attempts);

  char buf2[10];
  snprintf(buf2, sizeof(buf2), "%d", g_game_state.failed_attempts);

  glColor3f(STATUSBAR_DEFAULT_COLOR);
  glutStrokeString(TEXT_FONT, (unsigned char*) "Attempts: ");
  glColor3f(STATUSBAR_SUCCESS_COLOR);
  glutStrokeString(TEXT_FONT, (unsigned char*) &buf1[0]);
  glColor3f(STATUSBAR_DEFAULT_COLOR);
  glutStrokeString(TEXT_FONT, (unsigned char*) " / ");
  glColor3f(STATUSBAR_FAIL_COLOR);
  glutStrokeString(TEXT_FONT, (unsigned char*) &buf2[0]);
}

void DrawScore()
{
  char buf[20];
  snprintf(buf, sizeof(buf), " Score: %d", g_game_state.score);

  glColor3f(STATUSBAR_DEFAULT_COLOR);
  glutStrokeString(TEXT_FONT, (unsigned char*) &buf[0]);
}
