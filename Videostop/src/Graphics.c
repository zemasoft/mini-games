// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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

#include "Config.h"
#include "World.h"

#if defined(USE_GLFW)
extern GLFWwindow* g_window;
#endif

#if defined(USE_SDL2)
extern SDL_Window* g_window;
#endif

static void InitStrings();
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

static int GetWindowWidth();
static int GetWindowHeight();

static float GetLeft();
static float GetRight();
static float GetBottom();
static float GetTop();

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
  glOrtho(GetLeft(), GetRight(), GetBottom(), GetTop(), -1.0f, 1.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  InitStrings();
}

void G_Update()
{
  glClear(GL_COLOR_BUFFER_BIT);

  DrawDices();
  DrawStatusBar();

#if defined(USE_FREEGLUT)
  glutSwapBuffers();
#endif

#if defined(USE_GLFW)
  glfwSwapBuffers(g_window);
#endif

#if defined(USE_SDL2)
  SDL_GL_SwapWindow(g_window);
#endif
}

void G_Stop()
{
}

void G_Resize(int const width, int const height)
{
  glViewport(0, 0, width, height);
}

void InitStrings()
{
  static float xf;
  static float yf;
  static bool init = true;

  if (init)
  {
    xf = (GetRight() - GetLeft()) / (float) GetWindowWidth();
    yf = (GetTop() - GetBottom()) / (float) GetWindowHeight();
    init = false;
  }

  (void) xf;
  g_world.statusBar.string_height = (float) glutStrokeHeight(TEXT_FONT) * TEXT_SIZE * yf;
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

#if defined(USE_FREEGLUT) || defined(USE_FREEGLUT_FOR_TEXT)
  glTranslatef(MARGIN / 2.0f,
               GetBottom() + (STATUSBAR_SIZE - g_world.statusBar.string_height * 0.75f) / 2.0f,
               0.0f);
  glScalef(TEXT_SIZE / 100.0f, TEXT_SIZE / 100.0f, 1.0f);
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

#if defined(USE_GLFW) && !defined(USE_FREEGLUT_FOR_TEXT)
  // TODO
#endif

#if defined(USE_SDL2) && !defined(USE_FREEGLUT_FOR_TEXT)
  // TODO
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
  return -MARGIN / 2.0f - STATUSBAR_SIZE;
}

float GetTop()
{
  return DICE_SIZE + MARGIN / 2.0f;
}

int GetWindowWidth()
{
#if defined(USE_FREEGLUT)
  return glutGet(GLUT_WINDOW_WIDTH);
#endif

#if defined(USE_GLFW)
  int window_width;
  int window_height;
  glfwGetWindowSize(g_window, &window_width, &window_height);

  return window_width;
#endif

#if defined(USE_SDL2)
  int window_width;
  int window_height;
  SDL_GetWindowSize(g_window, &window_width, &window_height);

  return window_width;
#endif
}

int GetWindowHeight()
{
#if defined(USE_FREEGLUT)
  return glutGet(GLUT_WINDOW_HEIGHT);
#endif

#if defined(USE_GLFW)
  int window_width;
  int window_height;
  glfwGetWindowSize(g_window, &window_width, &window_height);

  return window_height;
#endif

#if defined(USE_SDL2)
  int window_width;
  int window_height;
  SDL_GetWindowSize(g_window, &window_width, &window_height);

  return window_height;
#endif
}
