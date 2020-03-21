// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Graphics.h"

#include <stdbool.h>  // bool, false, true
#include <stddef.h>   // size_t
#include <stdio.h>    // snprintf

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
static void DrawPieces();
static void DrawStatusBar();

static void InitPieceString(struct Piece* piece, float xf, float yf);

static void DrawPiece(struct Piece const* piece);
static void DrawValue(struct Piece const* piece);

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

  DrawPieces();
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

  for (size_t i = 0; i < g_world.piece_count; ++i)
  {
    InitPieceString(&g_world.pieces[i], xf, yf);
  }

  g_world.statusBar.string_height = (float) glutStrokeHeight(TEXT_FONT) * TEXT_SIZE * yf;
}

void DrawPieces()
{
  for (size_t i = 0; i < g_world.piece_count; ++i)
  {
    DrawPiece(&g_world.pieces[i]);
  }
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
    case WorldState_Moving:
      glColor3f(STATUSBAR_MOVING_COLOR);
      break;
    case WorldState_Success:
      glColor3f(STATUSBAR_SUCCESS_COLOR);
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
    glColor3f(STATUSBAR_TEXT_COLOR);

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
    char buf[30];
    snprintf(buf, sizeof(buf), "Moves: %d / %d", g_world.moves, g_world.single_moves);

    glColor3f(STATUSBAR_TEXT_COLOR);
    glutStrokeString(TEXT_FONT, (unsigned char*) &buf[0]);
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

void InitPieceString(struct Piece* const piece, float xf, float yf)
{
#if defined(USE_FREEGLUT) || defined(USE_FREEGLUT_FOR_TEXT)
  snprintf(piece->string.value, sizeof(piece->string.value), "%d", piece->value);

  piece->string.width =
      glutStrokeLengthf(TEXT_FONT, (unsigned char*) &piece->string.value[0]) * VALUE_SIZE * xf;
  piece->string.height = (float) glutStrokeHeight(TEXT_FONT) * VALUE_SIZE * yf;
#endif

#if defined(USE_GLFW) && !defined(USE_FREEGLUT_FOR_TEXT)
  // TODO
  (void) piece;
  (void) xf;
  (void) yf;
#endif

#if defined(USE_SDL2) && !defined(USE_FREEGLUT_FOR_TEXT)
  // TODO
  (void) piece;
  (void) xf;
  (void) yf;
#endif
}

void DrawPiece(struct Piece const* const piece)
{
  if (piece->value == 0)
  {
    return;
  }

  glPushMatrix();
  glTranslatef(piece->pos.x * PIECE_SIZE, piece->pos.y * PIECE_SIZE, 0.0f);

  float left = 0.0f;
  float top = PIECE_SIZE;
  float right = PIECE_SIZE;
  float bottom = 0.0f;

  switch (piece->state)
  {
    case PieceState_Setup:
      glColor3f(BORDER_SETUP_COLOR);
      break;
    case PieceState_Idle:
      glColor3f(BORDER_IDLE_COLOR);
      break;
    case PieceState_Moving:
      glColor3f(BORDER_MOVING_COLOR);
      break;
    case PieceState_Success:
      glColor3f(BORDER_SUCCESS_COLOR);
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

  switch (piece->state)
  {
    case PieceState_Setup:
      glColor3f(PIECE_SETUP_COLOR);
      break;
    case PieceState_Idle:
      glColor3f(PIECE_IDLE_COLOR);
      break;
    case PieceState_Moving:
      glColor3f(PIECE_MOVING_COLOR);
      break;
    case PieceState_Success:
      glColor3f(PIECE_SUCCESS_COLOR);
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

  DrawValue(piece);

  glPopMatrix();
}

void DrawValue(struct Piece const* const piece)
{
#if defined(USE_FREEGLUT) || defined(USE_FREEGLUT_FOR_TEXT)
  switch (piece->state)
  {
    case PieceState_Setup:
      glColor3f(VALUE_SETUP_COLOR);
      break;
    case PieceState_Idle:
      glColor3f(VALUE_IDLE_COLOR);
      break;
    case PieceState_Moving:
      glColor3f(VALUE_MOVING_COLOR);
      break;
    case PieceState_Success:
      glColor3f(VALUE_SUCCESS_COLOR);
      break;
  }

  glTranslatef((PIECE_SIZE - piece->string.width) / 2.0f,
               (PIECE_SIZE - piece->string.height * 0.75f) / 2.0f, 0.0f);
  glScalef(VALUE_SIZE / 100.0f, VALUE_SIZE / 100.0f, 1.0f);
  glLineWidth(1.5f);
  glutStrokeString(TEXT_FONT, (unsigned char*) &piece->string.value[0]);
#endif

#if defined(USE_GLFW) && !defined(USE_FREEGLUT_FOR_TEXT)
  // TODO
  (void) piece;
#endif

#if defined(USE_SDL2) && !defined(USE_FREEGLUT_FOR_TEXT)
  // TODO
  (void) piece;
#endif
}

float GetLeft()
{
  return -MARGIN / 2.0f;
}

float GetRight()
{
  return (float) g_world.size.x * PIECE_SIZE + MARGIN / 2.0f;
}

float GetBottom()
{
  return -MARGIN / 2.0f - STATUSBAR_SIZE;
}

float GetTop()
{
  return (float) g_world.size.y * PIECE_SIZE + MARGIN / 2.0f;
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
