#include "Graphics.h"

#include <stdbool.h>  // bool, false, true
#include <stddef.h>   // size_t
#include <stdio.h>    // snprintf

#include "zge/core.h"

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

static void RecountPieceStrings();
static void RecountStatusBarString();
static void DrawPieces();
static void DrawStatusBar();

static void RecountPieceString(struct Piece* piece);

static void DrawPiece(struct Piece const* piece);
static void DrawValue(struct Piece const* piece);

static float GetLeft();
static float GetRight();
static float GetBottom();
static float GetTop();

void G_Start()
{
  s_scale.x = 1.0f;
  s_scale.y = 1.0f;

  s_init_window_width = ZGE_GetWindowWidth();
  s_init_window_height = ZGE_GetWindowHeight();
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
  s_scale.x = (float) s_init_window_width / (float) ZGE_GetWindowWidth() *
              (GetRight() - GetLeft()) / s_init_width;
  s_scale.y = (float) s_init_window_height / (float) ZGE_GetWindowHeight() *
              (GetTop() - GetBottom()) / s_init_height;

  RecountPieceStrings();
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

  DrawPieces();
  DrawStatusBar();

  ZGE_SwapBuffers();
}

void G_Stop()
{
}

void G_Resize(int const width, int const height)
{
  glViewport(0, 0, width, height);

  G_Restart();
}

void RecountPieceStrings()
{
  for (size_t i = 0; i < g_world.piece_count; ++i)
  {
    RecountPieceString(&g_world.pieces[i]);
  }
}

void RecountStatusBarString()
{
#if defined(USE_FREEGLUT_FOR_TEXT)
  s_statusBar.string.height = (float) glutStrokeHeight(TEXT_FONT) * TEXT_SIZE * s_string_scale.y;
#endif
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

#if defined(USE_FREEGLUT_FOR_TEXT)
  glTranslatef(
      GetLeft() + MARGIN * s_scale.x,
      GetBottom() + (STATUSBAR_SIZE - s_statusBar.string.height * 0.75f) / 2.0f * s_scale.y, 0.0f);
  glScalef(TEXT_SIZE / 100.0f * s_scale.x, TEXT_SIZE / 100.0f * s_scale.y, 1.0f);
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

  glPopMatrix();
}

void RecountPieceString(struct Piece* const piece)
{
#if defined(USE_FREEGLUT_FOR_TEXT)
  snprintf(piece->string.value, sizeof(piece->string.value), "%d", piece->value);

  piece->string.width = glutStrokeLengthf(TEXT_FONT, (unsigned char*) &piece->string.value[0]) *
                        VALUE_SIZE * s_string_scale.x;
  piece->string.height = (float) glutStrokeHeight(TEXT_FONT) * VALUE_SIZE * s_string_scale.y;
#endif

#if !defined(USE_FREEGLUT_FOR_TEXT)
  (void) piece;
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
#if defined(USE_FREEGLUT_FOR_TEXT)
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

#if !defined(USE_FREEGLUT_FOR_TEXT)
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
  return -MARGIN / 2.0f - STATUSBAR_SIZE * s_scale.y;
}

float GetTop()
{
  return (float) g_world.size.y * PIECE_SIZE + MARGIN / 2.0f;
}
