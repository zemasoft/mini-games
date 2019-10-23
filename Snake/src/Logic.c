// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Logic.h"

#include <assert.h>  // assert
#include <stddef.h>  // size_t
#include <stdlib.h>  // free, malloc, srand, rand
#include <time.h>    // time

#include <GLFW/glfw3.h>

#include "GameState.h"
#include "Input.h"
#include "Sound.h"

enum MoveSnake
{
  MoveSnake_Ok,
  MoveSnake_Food,
  MoveSnake_Wall,
  MoveSnake_Body,
  MoveSnake_NoSpace
};

extern GLFWwindow* g_window;

static enum MoveSnake MoveSnake();
static void PlaceFood();

static struct Field* GetField(int x, int y);

void L_Start()
{
  srand((unsigned int) time(NULL));

  g_game_state.field_count = (size_t)(g_game_state.size.x * g_game_state.size.y);
  g_game_state.fields = malloc(g_game_state.field_count * sizeof(struct Field));
  g_game_state.snake = malloc(g_game_state.field_count * sizeof(struct Field*));

  L_Restart();
}

void L_Restart()
{
  g_game_state.field_count = (size_t)(g_game_state.size.x * g_game_state.size.y);

  for (int y = 0; y < g_game_state.size.y; ++y)
  {
    for (int x = 0; x < g_game_state.size.x; ++x)
    {
      struct Field* field = GetField(x, y);

      field->pos.x = x;
      field->pos.y = y;
      field->val = Value_Empty;
    }
  }

  struct Field* head = GetField(g_game_state.size.x / 2, g_game_state.size.y / 2);

  assert(head->val == Value_Empty);

  head->val = Value_Snake;

  g_game_state.head = 0;
  g_game_state.tail = 0;
  g_game_state.snake[g_game_state.head++] = head;

  g_game_state.heading = Heading_Up;

  g_game_state.state = State_Play;

  g_game_state.max_move_time = 0.25;

  g_game_state.score = 0;

  PlaceFood();
}

void L_Update()
{
  static double before;
  static double move_time;

  double now = glfwGetTime();
  double elapsed = now - before;
  before = now;

  if (I_ResetKey())
  {
    I_Restart();
    L_Restart();
    S_Restart();
    // G_Restart();

    move_time = 0.0;
    return;
  }

  if (g_game_state.state == State_Play)
  {
    if (I_PauseKey())
    {
      g_game_state.state = State_Pause;
    }
    else
    {
      move_time += elapsed;

      if (move_time >= g_game_state.max_move_time)
      {
        move_time = 0.0;

        switch (I_PopDirectionKey())
        {
          case GLFW_KEY_LEFT:
            if (g_game_state.heading != Heading_Right)
            {
              g_game_state.heading = Heading_Left;
            }
            break;

          case GLFW_KEY_RIGHT:
            if (g_game_state.heading != Heading_Left)
            {
              g_game_state.heading = Heading_Right;
            }
            break;

          case GLFW_KEY_DOWN:
            if (g_game_state.heading != Heading_Up)
            {
              g_game_state.heading = Heading_Down;
            }
            break;

          case GLFW_KEY_UP:
            if (g_game_state.heading != Heading_Down)
            {
              g_game_state.heading = Heading_Up;
            }
            break;
        }

        switch (MoveSnake())
        {
          case MoveSnake_Ok:
            break;

          case MoveSnake_Food:
            S_PlaySound(Sound_Food);

            ++g_game_state.score;
            PlaceFood();
            break;

          case MoveSnake_Wall:
            S_PlaySound(Sound_Wall);

            g_game_state.state = State_Fail;
            break;

          case MoveSnake_Body:
            S_PlaySound(Sound_Body);

            g_game_state.state = State_Fail;
            break;

          case MoveSnake_NoSpace:
            S_PlaySound(Sound_Success);

            g_game_state.state = State_Success;
            break;
        }
      }
    }
  }
  else if (g_game_state.state == State_Success)
  {
  }
  else if (g_game_state.state == State_Fail)
  {
  }
  else if (g_game_state.state == State_Pause)
  {
    if (I_PauseKey())
    {
      g_game_state.state = State_Play;
    }
  }
}

void L_Stop()
{
  free(g_game_state.fields);
  free(g_game_state.snake);
}

enum MoveSnake MoveSnake()
{
  assert(g_game_state.head != g_game_state.tail);

  struct Field* head;

  if (g_game_state.head > 0)
  {
    head = g_game_state.snake[g_game_state.head - 1];
  }
  else
  {
    head = g_game_state.snake[g_game_state.field_count - 1];
  }

  int new_x_pos = head->pos.x;
  int new_y_pos = head->pos.y;

  switch (g_game_state.heading)
  {
    case Heading_Left:
      if (head->pos.x <= 0)
      {
        return MoveSnake_Wall;
      }

      --new_x_pos;
      break;

    case Heading_Right:
      if (head->pos.x >= g_game_state.size.x - 1)
      {
        return MoveSnake_Wall;
      }

      ++new_x_pos;
      break;

    case Heading_Down:
      if (head->pos.y <= 0)
      {
        return MoveSnake_Wall;
      }

      --new_y_pos;
      break;

    case Heading_Up:
      if (head->pos.y >= g_game_state.size.y - 1)
      {
        return MoveSnake_Wall;
      }

      ++new_y_pos;
      break;
  }

  enum MoveSnake res = MoveSnake_Ok;

  struct Field* new_head = GetField(new_x_pos, new_y_pos);

  switch (new_head->val)
  {
    case Value_Empty:
      break;

    case Value_Food:
      res = MoveSnake_Food;
      break;

    case Value_Wall:
      return MoveSnake_Wall;

    case Value_Snake:
      return MoveSnake_Body;
  }

  new_head->val = Value_Snake;

  g_game_state.snake[g_game_state.head++] = new_head;

  if (g_game_state.head >= g_game_state.field_count)
  {
    g_game_state.head = 0;
  }

  if (res == MoveSnake_Food)
  {
    if (g_game_state.head == g_game_state.tail)
    {
      return MoveSnake_NoSpace;
    }
  }
  else
  {
    struct Field* tail = g_game_state.snake[g_game_state.tail++];

    if (g_game_state.tail >= g_game_state.field_count)
    {
      g_game_state.tail = 0;
    }

    tail->val = Value_Empty;
  }

  return res;
}

void PlaceFood()
{
  struct Field* field;

  do
  {
    field = GetField(rand() % g_game_state.size.x, rand() % g_game_state.size.y);
  } while (field->val != Value_Empty);

  field->val = Value_Food;
}

struct Field* GetField(int x, int y)
{
  assert(x >= 0 && x < g_game_state.size.x);
  assert(y >= 0 && y < g_game_state.size.y);

  return &g_game_state.fields[x + y * g_game_state.size.x];
}
