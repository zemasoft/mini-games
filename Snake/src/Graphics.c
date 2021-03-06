#include "Graphics.h"

#include <math.h>    // M_PI, cos, sin
#include <stddef.h>  // NULL, size_t

#include "zge/assert.h"
#include "zge/core.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

#if defined(USE_FREEGLUT)
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
#include "Tools.h"
#include "World.h"

static void DrawStatusBar();
static void DrawMargin();
static void DrawGround();
static void DrawSnake();

static void DrawField(struct Field const* field);
static void DrawFood();

static void DrawSnakeElement(size_t index);

static void DrawSnakeHeadUp(struct Field const* head);
static void DrawSnakeHeadDown(struct Field const* head);
static void DrawSnakeHeadLeft(struct Field const* head);
static void DrawSnakeHeadRight(struct Field const* head);

static void DrawSnakeTailUp(struct Field const* tail);
static void DrawSnakeTailDown(struct Field const* tail);
static void DrawSnakeTailLeft(struct Field const* tail);
static void DrawSnakeTailRight(struct Field const* tail);

static void DrawSnakeBody(struct Field const* body);

void G_Start()
{
  G_Restart();
}

void G_Restart()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-MARGIN_SIZE, (float) g_world.ground.size.x * FIELD_SIZE + MARGIN_SIZE, -MARGIN_SIZE,
          (float) g_world.ground.size.y * FIELD_SIZE + MARGIN_SIZE + STATUSBAR_SIZE, -1.0f, 1.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void G_Update()
{
  glClear(GL_COLOR_BUFFER_BIT);

  DrawStatusBar();
  DrawMargin();
  DrawGround();
  DrawSnake();

  ZGE_SwapBuffers();
}

void G_Stop()
{
}

void DrawStatusBar()
{
  glPushMatrix();
  glTranslatef(-MARGIN_SIZE, (float) g_world.ground.size.y * FIELD_SIZE + MARGIN_SIZE, 0.0f);

  float const left = 0.0f;
  float const top = STATUSBAR_SIZE;
  float const right = MARGIN_SIZE + (float) g_world.ground.size.x * FIELD_SIZE + MARGIN_SIZE;
  float const bottom = 0.0f;

  glColor3ub(STATUSBAR_COLOR);

  // clang-format off
  glBegin(GL_POLYGON);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
  glEnd();
  // clang-format on

  glPopMatrix();
}

void DrawMargin()
{
  float const left = -MARGIN_SIZE;
  float const top = (float) g_world.ground.size.y * FIELD_SIZE + MARGIN_SIZE;
  float const right = (float) g_world.ground.size.x * FIELD_SIZE + MARGIN_SIZE;
  float const bottom = -MARGIN_SIZE;

  glColor3ub(MARGIN_COLOR);

  // clang-format off
  glBegin(GL_POLYGON);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
  glEnd();
  // clang-format on
}

void DrawGround()
{
  for (size_t i = 0; i < g_world.ground.fieldCount; ++i)
  {
    DrawField(&g_world.ground.fields[i]);
  }
}

void DrawSnake()
{
  for (size_t index = g_world.snake.tail; index != g_world.snake.head;)
  {
    DrawSnakeElement(index);

    index = GetNextSnakeIndex(index);
  }
}

void DrawField(struct Field const* const field)
{
  glPushMatrix();
  glTranslatef((float) field->pos.x * FIELD_SIZE, (float) field->pos.y * FIELD_SIZE, 0.0f);

  float const left = 0.0f;
  float const top = FIELD_SIZE;
  float const right = FIELD_SIZE;
  float const bottom = 0.0f;

  if (field->pos.x % 2 == 0)
  {
    if (field->pos.y % 2 == 0)
    {
      glColor3ub(LIGHT_FIELD_COLOR);
    }
    else
    {
      glColor3ub(DARK_FIELD_COLOR);
    }
  }
  else
  {
    if (field->pos.y % 2 == 0)
    {
      glColor3ub(DARK_FIELD_COLOR);
    }
    else
    {
      glColor3ub(LIGHT_FIELD_COLOR);
    }
  }

  // clang-format off
  glBegin(GL_POLYGON);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
  glEnd();
  // clang-format on

  if (field->value == FieldValue_Food)
  {
    DrawFood();
  }
  else if (field->value == FieldValue_Wall)
  {
    // TODO
  }

  glPopMatrix();
}

void DrawFood()
{
  glTranslatef(FIELD_SIZE / 2.0f, FIELD_SIZE / 2.0f, 0.0f);

  glColor3ub(FOOD_COLOR);

  glBegin(GL_POLYGON);

  for (int i = 0; i < 360; i += 15)
  {
    glVertex2f(FOOD_RADIUS * cosf((float) (i * M_PI) / 180.0f),
               FOOD_RADIUS * sinf((float) (i * M_PI) / 180.0f));
  }

  glEnd();
}

void DrawSnakeElement(size_t const index)
{
  struct Field const* const e = GetSnakeElement(index);

  ZGE_AssertDebug(e != NULL);

  struct Field const* const prevE = GetPrevSnakeElement(index);
  struct Field const* const nextE = GetNextSnakeElement(index);

  ZGE_AssertDebug(prevE != NULL || nextE != NULL);

  if (prevE == NULL)
  {
    ZGE_AssertDebug((nextE->pos.x == e->pos.x && nextE->pos.y != e->pos.y) ||
                    (nextE->pos.x != e->pos.x && nextE->pos.y == e->pos.y));

    if (nextE->pos.x == e->pos.x)
    {
      if (nextE->pos.y < e->pos.y)
      {
        DrawSnakeTailUp(e);
      }
      else
      {
        DrawSnakeTailDown(e);
      }
    }
    else
    {
      if (nextE->pos.x > e->pos.x)
      {
        DrawSnakeTailLeft(e);
      }
      else
      {
        DrawSnakeTailRight(e);
      }
    }
  }
  else if (nextE == NULL)
  {
    ZGE_AssertDebug((prevE->pos.x == e->pos.x && prevE->pos.y != e->pos.y) ||
                    (prevE->pos.x != e->pos.x && prevE->pos.y == e->pos.y));

    if (prevE->pos.x == e->pos.x)
    {
      if (prevE->pos.y < e->pos.y)
      {
        DrawSnakeHeadUp(e);
      }
      else
      {
        DrawSnakeHeadDown(e);
      }
    }
    else
    {
      if (prevE->pos.x > e->pos.x)
      {
        DrawSnakeHeadLeft(e);
      }
      else
      {
        DrawSnakeHeadRight(e);
      }
    }
  }
  else
  {
    ZGE_AssertDebug((prevE->pos.x == e->pos.x && prevE->pos.y != e->pos.y) ||
                    (prevE->pos.x != e->pos.x && prevE->pos.y == e->pos.y));
    ZGE_AssertDebug((nextE->pos.x == e->pos.x && nextE->pos.y != e->pos.y) ||
                    (nextE->pos.x != e->pos.x && nextE->pos.y == e->pos.y));
    ZGE_AssertDebug(prevE->pos.x != nextE->pos.x || prevE->pos.y != nextE->pos.y);

    DrawSnakeBody(e);
  }
}

void DrawSnakeHeadUp(struct Field const* const head)
{
  glPushMatrix();
  glTranslatef((float) head->pos.x * FIELD_SIZE, (float) head->pos.y * FIELD_SIZE, 0.0f);

  float const left = 0.0f;
  float const top = FIELD_SIZE * g_world.snake.headOffset;
  float const right = FIELD_SIZE;
  float const bottom = 0.0f;

  glColor3ub(SNAKE_COLOR);

  // clang-format off
  glBegin(GL_POLYGON);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
  glEnd();
  // clang-format on

  glPopMatrix();
}

void DrawSnakeHeadDown(struct Field const* const head)
{
  glPushMatrix();
  glTranslatef((float) head->pos.x * FIELD_SIZE, (float) head->pos.y * FIELD_SIZE, 0.0f);

  float const left = 0.0f;
  float const top = FIELD_SIZE;
  float const right = FIELD_SIZE;
  float const bottom = FIELD_SIZE * (1.0f - g_world.snake.headOffset);

  glColor3ub(SNAKE_COLOR);

  // clang-format off
  glBegin(GL_POLYGON);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
  glEnd();
  // clang-format on

  glPopMatrix();
}

void DrawSnakeHeadLeft(struct Field const* const head)
{
  glPushMatrix();
  glTranslatef((float) head->pos.x * FIELD_SIZE, (float) head->pos.y * FIELD_SIZE, 0.0f);

  float const left = FIELD_SIZE * (1.0f - g_world.snake.headOffset);
  float const top = FIELD_SIZE;
  float const right = FIELD_SIZE;
  float const bottom = 0.0f;

  glColor3ub(SNAKE_COLOR);

  // clang-format off
  glBegin(GL_POLYGON);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
  glEnd();
  // clang-format on

  glPopMatrix();
}

void DrawSnakeHeadRight(struct Field const* const head)
{
  glPushMatrix();
  glTranslatef((float) head->pos.x * FIELD_SIZE, (float) head->pos.y * FIELD_SIZE, 0.0f);

  float const left = 0.0f;
  float const top = FIELD_SIZE;
  float const right = FIELD_SIZE * g_world.snake.headOffset;
  float const bottom = 0.0f;

  glColor3ub(SNAKE_COLOR);

  // clang-format off
  glBegin(GL_POLYGON);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
  glEnd();
  // clang-format on

  glPopMatrix();
}

void DrawSnakeTailUp(struct Field const* const tail)
{
  glPushMatrix();
  glTranslatef((float) tail->pos.x * FIELD_SIZE, (float) tail->pos.y * FIELD_SIZE, 0.0f);

  float const left = 0.0f;
  float const top = FIELD_SIZE * (1.0f - g_world.snake.tailOffset);
  float const right = FIELD_SIZE;
  float const bottom = 0.0f;

  glColor3ub(SNAKE_COLOR);

  // clang-format off
  glBegin(GL_POLYGON);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
  glEnd();
  // clang-format on

  glPopMatrix();
}

void DrawSnakeTailDown(struct Field const* const tail)
{
  glPushMatrix();
  glTranslatef((float) tail->pos.x * FIELD_SIZE, (float) tail->pos.y * FIELD_SIZE, 0.0f);

  float const left = 0.0f;
  float const top = FIELD_SIZE;
  float const right = FIELD_SIZE;
  float const bottom = FIELD_SIZE * g_world.snake.tailOffset;

  glColor3ub(SNAKE_COLOR);

  // clang-format off
  glBegin(GL_POLYGON);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
  glEnd();
  // clang-format on

  glPopMatrix();
}

void DrawSnakeTailLeft(struct Field const* const tail)
{
  glPushMatrix();
  glTranslatef((float) tail->pos.x * FIELD_SIZE, (float) tail->pos.y * FIELD_SIZE, 0.0f);

  float const left = FIELD_SIZE * g_world.snake.tailOffset;
  float const top = FIELD_SIZE;
  float const right = FIELD_SIZE;
  float const bottom = 0.0f;

  glColor3ub(SNAKE_COLOR);

  // clang-format off
  glBegin(GL_POLYGON);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
  glEnd();
  // clang-format on

  glPopMatrix();
}

void DrawSnakeTailRight(struct Field const* const tail)
{
  glPushMatrix();
  glTranslatef((float) tail->pos.x * FIELD_SIZE, (float) tail->pos.y * FIELD_SIZE, 0.0f);

  float const left = 0.0f;
  float const top = FIELD_SIZE;
  float const right = FIELD_SIZE * (1.0f - g_world.snake.tailOffset);
  float const bottom = 0.0f;

  glColor3ub(SNAKE_COLOR);

  // clang-format off
  glBegin(GL_POLYGON);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
  glEnd();
  // clang-format on

  glPopMatrix();
}

void DrawSnakeBody(struct Field const* const body)
{
  glPushMatrix();
  glTranslatef((float) body->pos.x * FIELD_SIZE, (float) body->pos.y * FIELD_SIZE, 0.0f);

  float const left = 0.0f;
  float const top = FIELD_SIZE;
  float const right = FIELD_SIZE;
  float const bottom = 0.0f;

  glColor3ub(SNAKE_COLOR);

  // clang-format off
  glBegin(GL_POLYGON);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
  glEnd();
  // clang-format on

  glPopMatrix();
}
