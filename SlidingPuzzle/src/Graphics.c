// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Graphics.h"

#include <stdio.h>  // snprintf

#include <GL/freeglut.h>

#include "GameState.h"

#define PIECE_SIZE 1.0f
#define MARGIN 0.02f * PIECE_SIZE
#define BORDER 0.05f * PIECE_SIZE
#define VALUE_SIZE 0.30f * PIECE_SIZE
#define STATUSBAR_SIZE 0.24f * PIECE_SIZE
#define TEXT_SIZE 0.14f * PIECE_SIZE

#define BORDER_IDLE_COLOR 1.0f, 1.0f, 1.0f
#define BORDER_MOVING_COLOR 1.0f, 1.0f, 1.0f
#define BORDER_SUCCESS_COLOR 0.0f, 0.5f, 0.0f

#define PIECE_IDLE_COLOR 0.0f, 0.0f, 0.5f
#define PIECE_MOVING_COLOR 0.0f, 0.0f, 0.5f
#define PIECE_SUCCESS_COLOR 0.0f, 0.0f, 0.5f

#define VALUE_IDLE_COLOR 1.0f, 1.0f, 1.0f
#define VALUE_MOVING_COLOR 1.0f, 1.0f, 1.0f
#define VALUE_SUCCESS_COLOR 1.0f, 1.0f, 1.0f

#define STATUSBAR_COLOR 1.0f, 1.0f, 1.0f

struct Projection
{
  float left;
  float right;
  float bottom;
  float top;
};

static void InitPieceStrings(const struct Projection* projection);
static void DrawPieces();
static void DrawStatusBar();

static void InitPieceString(struct Piece* piece, const struct Projection* projection);

static void DrawPiece(const struct Piece* piece);
static void DrawValue(const struct Piece* piece);

static void DrawMoves();

void G_Start()
{
  struct Projection projection;
  projection.left = -MARGIN / 2.0f;
  projection.right = (float) g_game_state.x_size * PIECE_SIZE + MARGIN / 2.0f;
  projection.bottom = -MARGIN / 2.0f - STATUSBAR_SIZE;
  projection.top = (float) g_game_state.y_size * PIECE_SIZE + MARGIN / 2.0f;

  InitPieceStrings(&projection);

  glEnable(GL_MULTISAMPLE);

  glDisable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(projection.left, projection.right, projection.bottom, projection.top, -1.0f, 1.0f);

  glMatrixMode(GL_MODELVIEW);
}

void G_Update()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  DrawPieces();
  DrawStatusBar();

  glutSwapBuffers();
}

void G_Stop()
{
}

void InitPieceStrings(const struct Projection* projection)
{
  for (size_t i = 0; i < g_game_state.piece_count; ++i)
  {
    InitPieceString(&g_game_state.pieces[i], projection);
  }
}

void DrawPieces()
{
  for (size_t i = 0; i < g_game_state.piece_count; ++i)
  {
    DrawPiece(&g_game_state.pieces[i]);
  }
}

void DrawStatusBar()
{
  glPushMatrix();

  glTranslatef(MARGIN / 2.0f, -MARGIN / 2.0f - TEXT_SIZE - 0.05f * PIECE_SIZE, 0.0f);
  glScalef(TEXT_SIZE / 100.0f, TEXT_SIZE / 100.0f, 1.0f);
  glLineWidth(1.2f);

  DrawMoves();

  glPopMatrix();
}

void InitPieceString(struct Piece* piece, const struct Projection* projection)
{
  snprintf(piece->string, sizeof(piece->string), "%d", piece->value);

  piece->string_width =
      (float) glutStrokeLength(GLUT_STROKE_MONO_ROMAN, (unsigned char*) &piece->string[0]) /
      (float) glutGet(GLUT_WINDOW_WIDTH) * (projection->right - projection->left) * VALUE_SIZE;

  piece->string_height = (float) glutStrokeHeight(GLUT_STROKE_MONO_ROMAN) /
                         (float) glutGet(GLUT_WINDOW_HEIGHT) *
                         (projection->top - projection->bottom) * VALUE_SIZE;
}

void DrawPiece(const struct Piece* piece)
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
    case State_Idle:
      glColor3f(BORDER_IDLE_COLOR);
      break;
    case State_Moving:
      glColor3f(BORDER_MOVING_COLOR);
      break;
    case State_Success:
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
    case State_Idle:
      glColor3f(PIECE_IDLE_COLOR);
      break;
    case State_Moving:
      glColor3f(PIECE_MOVING_COLOR);
      break;
    case State_Success:
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

void DrawValue(const struct Piece* piece)
{
  switch (piece->state)
  {
    case State_Idle:
      glColor3f(VALUE_IDLE_COLOR);
      break;
    case State_Moving:
      glColor3f(VALUE_MOVING_COLOR);
      break;
    case State_Success:
      glColor3f(VALUE_SUCCESS_COLOR);
      break;
  }

  glTranslatef((PIECE_SIZE - piece->string_width) / 2.0f,
               (PIECE_SIZE - piece->string_height * 0.75f) / 2.0f, 0.0f);
  glScalef(VALUE_SIZE / 100.0f, VALUE_SIZE / 100.0f, 1.0f);
  glLineWidth(1.5f);
  glutStrokeString(GLUT_STROKE_MONO_ROMAN, (unsigned char*) &piece->string[0]);
}

void DrawMoves()
{
  char moves_buf[30];
  snprintf(moves_buf, sizeof(moves_buf), "Moves: %d / %d", g_game_state.moves,
           g_game_state.single_moves);

  glColor3f(STATUSBAR_COLOR);
  glutStrokeString(GLUT_STROKE_MONO_ROMAN, (unsigned char*) &moves_buf[0]);
}
