// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Graphics.h"

#include <assert.h>  // assert
#include <math.h>    // M_PI, cos, sin
#include <stddef.h>  // size_t

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

#if defined(USE_GLFW)
extern GLFWwindow* g_window;
#endif

#if defined(USE_SDL2)
extern SDL_Window* g_window;
#endif

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
#if defined(USE_SDL2)
  SDL_GL_CreateContext(g_window);
  SDL_GL_SetSwapInterval(1);
#endif

  G_Restart();
}

void G_Restart()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-MARGIN_SIZE, (float) g_world.ground.size.x * FIELD_SIZE + MARGIN_SIZE, -MARGIN_SIZE,
          (float) g_world.ground.size.y * FIELD_SIZE + MARGIN_SIZE + STATUSBAR_SIZE, -1.0f, 1.0f);

  glMatrixMode(GL_MODELVIEW);
}

void G_Update()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  DrawStatusBar();
  DrawMargin();
  DrawGround();
  DrawSnake();

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
  for (size_t i = 0; i < g_world.ground.field_count; ++i)
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

  assert(e != NULL);

  struct Field const* const prev_e = GetPrevSnakeElement(index);
  struct Field const* const next_e = GetNextSnakeElement(index);

  assert(prev_e != NULL || next_e != NULL);

  if (prev_e == NULL)
  {
    assert((next_e->pos.x == e->pos.x && next_e->pos.y != e->pos.y) ||
           (next_e->pos.x != e->pos.x && next_e->pos.y == e->pos.y));

    if (next_e->pos.x == e->pos.x)
    {
      if (next_e->pos.y < e->pos.y)
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
      if (next_e->pos.x > e->pos.x)
      {
        DrawSnakeTailLeft(e);
      }
      else
      {
        DrawSnakeTailRight(e);
      }
    }
  }
  else if (next_e == NULL)
  {
    assert((prev_e->pos.x == e->pos.x && prev_e->pos.y != e->pos.y) ||
           (prev_e->pos.x != e->pos.x && prev_e->pos.y == e->pos.y));

    if (prev_e->pos.x == e->pos.x)
    {
      if (prev_e->pos.y < e->pos.y)
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
      if (prev_e->pos.x > e->pos.x)
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
    assert((prev_e->pos.x == e->pos.x && prev_e->pos.y != e->pos.y) ||
           (prev_e->pos.x != e->pos.x && prev_e->pos.y == e->pos.y));
    assert((next_e->pos.x == e->pos.x && next_e->pos.y != e->pos.y) ||
           (next_e->pos.x != e->pos.x && next_e->pos.y == e->pos.y));
    assert(prev_e->pos.x != next_e->pos.x || prev_e->pos.y != next_e->pos.y);

    DrawSnakeBody(e);
  }
}

void DrawSnakeHeadUp(struct Field const* const head)
{
  glPushMatrix();
  glTranslatef((float) head->pos.x * FIELD_SIZE, (float) head->pos.y * FIELD_SIZE, 0.0f);

  float const left = 0.0f;
  float const top = FIELD_SIZE * g_world.snake.head_offset;
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
  float const bottom = FIELD_SIZE * (1.0f - g_world.snake.head_offset);

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

  float const left = FIELD_SIZE * (1.0f - g_world.snake.head_offset);
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
  float const right = FIELD_SIZE * g_world.snake.head_offset;
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
  float const top = FIELD_SIZE * (1.0f - g_world.snake.tail_offset);
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
  float const bottom = FIELD_SIZE * g_world.snake.tail_offset;

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

  float const left = FIELD_SIZE * g_world.snake.tail_offset;
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
  float const right = FIELD_SIZE * (1.0f - g_world.snake.tail_offset);
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
