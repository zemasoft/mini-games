// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Graphics.h"

#include <stddef.h>  // size_t
#include <stdio.h>   // snprintf

#include <GL/freeglut.h>

#include "Config.h"
#include "World.h"

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
  glEnable(GL_MULTISAMPLE);
  glDisable(GL_DEPTH_TEST);

  G_Restart();
}

void G_Restart()
{
  struct Projection projection;
  projection.left = -MARGIN / 2.0f;
  projection.right = (float) g_world.size.x * PIECE_SIZE + MARGIN / 2.0f;
  projection.bottom = -MARGIN / 2.0f - STATUSBAR_SIZE;
  projection.top = (float) g_world.size.y * PIECE_SIZE + MARGIN / 2.0f;

  InitPieceStrings(&projection);

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

void InitPieceStrings(const struct Projection* const projection)
{
  for (size_t i = 0; i < g_world.piece_count; ++i)
  {
    InitPieceString(&g_world.pieces[i], projection);
  }
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

  glTranslatef(MARGIN / 2.0f, -MARGIN / 2.0f - TEXT_SIZE - 0.05f * PIECE_SIZE, 0.0f);
  glScalef(TEXT_SIZE / 100.0f, TEXT_SIZE / 100.0f, 1.0f);
  glLineWidth(1.2f);

  if (g_world.state == WorldState_Setup)
  {
    glColor3f(STATUSBAR_COLOR);

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
    DrawMoves();
  }

  glPopMatrix();
}

void InitPieceString(struct Piece* const piece, const struct Projection* const projection)
{
  snprintf(piece->string.value, sizeof(piece->string.value), "%d", piece->value);

  piece->string.width =
      (float) glutStrokeLength(TEXT_FONT, (unsigned char*) &piece->string.value[0]) /
      (float) glutGet(GLUT_WINDOW_WIDTH) * (projection->right - projection->left) * VALUE_SIZE;

  piece->string.height = (float) glutStrokeHeight(TEXT_FONT) / (float) glutGet(GLUT_WINDOW_HEIGHT) *
                         (projection->top - projection->bottom) * VALUE_SIZE;
}

void DrawPiece(const struct Piece* const piece)
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

void DrawValue(const struct Piece* const piece)
{
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
}

void DrawMoves()
{
  char buf[30];
  snprintf(buf, sizeof(buf), "Moves: %d / %d", g_world.moves, g_world.single_moves);

  glColor3f(STATUSBAR_COLOR);
  glutStrokeString(TEXT_FONT, (unsigned char*) &buf[0]);
}
