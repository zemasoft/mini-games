// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Graphics.h"

#include <assert.h>  // assert
#include <math.h>    // M_PI, cos, sin
#include <stddef.h>  // size_t

#include <GLFW/glfw3.h>

#include "GameConfig.h"
#include "GameState.h"

extern GLFWwindow* g_window;

static void DrawFields();
static void DrawSnake();

static void DrawField(const struct Field* field);
static void DrawFood();

static void DrawHead();
static void DrawBody();
static void DrawTail();

void G_Start()
{
  G_Restart();
}

void G_Restart()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0f, (float) g_game_state.size.x * FIELD_SIZE, 0.0f,
          (float) g_game_state.size.y * FIELD_SIZE, -1.0f, 1.0f);

  glMatrixMode(GL_MODELVIEW);
}

void G_Update()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  DrawFields();
  DrawSnake();

  glfwSwapBuffers(g_window);
}

void G_Stop()
{
}

void DrawFields()
{
  for (size_t i = 0; i < g_game_state.field_count; ++i)
  {
    DrawField(&g_game_state.fields[i]);
  }
}

void DrawSnake()
{
  DrawHead();
  DrawBody();
  DrawTail();
}

void DrawField(const struct Field* field)
{
  glPushMatrix();
  glTranslatef((float) field->pos.x * FIELD_SIZE, (float) field->pos.y * FIELD_SIZE, 0.0f);

  float left = 0.0f;
  float top = FIELD_SIZE;
  float right = FIELD_SIZE;
  float bottom = 0.0f;

  if (field->pos.x % 2 == 0)
  {
    if (field->pos.y % 2 == 0)
    {
      glColor3f(LIGHT_FIELD_COLOR);
    }
    else
    {
      glColor3f(DARK_FIELD_COLOR);
    }
  }
  else
  {
    if (field->pos.y % 2 == 0)
    {
      glColor3f(DARK_FIELD_COLOR);
    }
    else
    {
      glColor3f(LIGHT_FIELD_COLOR);
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

  if (field->val == Value_Food)
  {
    DrawFood();
  }
  else if (field->val == Value_Wall)
  {
    // TODO
  }

  glPopMatrix();
}

void DrawFood()
{
  glTranslatef(FIELD_SIZE / 2.0f, FIELD_SIZE / 2.0f, 0.0f);

  glColor3f(FOOD_COLOR);

  glBegin(GL_POLYGON);

  for (int i = 0; i < 360; i += 15)
  {
    glVertex2f(FOOD_RADIUS * cosf((float) (i * M_PI) / 180.0f),
               FOOD_RADIUS * sinf((float) (i * M_PI) / 180.0f));
  }

  glEnd();
}

void DrawHead()
{
  struct Field* head;

  if (g_game_state.head > 0)
  {
    head = g_game_state.snake[g_game_state.head - 1];
  }
  else
  {
    head = g_game_state.snake[g_game_state.snake_count - 1];
  }

  assert(head->val == Value_Snake);

  glPushMatrix();
  glTranslatef((float) head->pos.x * FIELD_SIZE, (float) head->pos.y * FIELD_SIZE, 0.0f);

  glColor3f(HEAD_COLOR);

  // clang-format off
  glBegin(GL_POLYGON);
    glVertex2f(0.0f, 1.0f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(1.0f, 1.0f);
  glEnd();
  // clang-format on

  glPopMatrix();
}

void DrawBody()
{
  size_t body_index;

  if (g_game_state.tail < g_game_state.snake_count - 1)
  {
    body_index = g_game_state.tail + 1;
  }
  else
  {
    body_index = 0;
  }

  assert(body_index != g_game_state.head);

  size_t head_index;

  if (g_game_state.head > 0)
  {
    head_index = g_game_state.head - 1;
  }
  else
  {
    head_index = g_game_state.snake_count - 1;
  }

  while (body_index != head_index)
  {
    const struct Field* body = g_game_state.snake[body_index++];

    if (body_index > g_game_state.snake_count - 1)
    {
      body_index = 0;
    }

    assert(body->val == Value_Snake);

    glPushMatrix();
    glTranslatef((float) body->pos.x * FIELD_SIZE, (float) body->pos.y * FIELD_SIZE, 0.0f);

    glColor3f(BODY_COLOR);

    // clang-format off
    glBegin(GL_POLYGON);
      glVertex2f(0.0f, 1.0f);
      glVertex2f(0.0f, 0.0f);
      glVertex2f(1.0f, 0.0f);
      glVertex2f(1.0f, 1.0f);
    glEnd();
    // clang-format on

    glPopMatrix();
  }
}

void DrawTail()
{
  const struct Field* tail = g_game_state.snake[g_game_state.tail];

  assert(tail->val == Value_Snake);

  glPushMatrix();
  glTranslatef((float) tail->pos.x * FIELD_SIZE, (float) tail->pos.y * FIELD_SIZE, 0.0f);

  glColor3f(TAIL_COLOR);

  // clang-format off
  glBegin(GL_POLYGON);
    glVertex2f(0.0f, 1.0f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(1.0f, 1.0f);
  glEnd();
  // clang-format on

  glPopMatrix();
}
