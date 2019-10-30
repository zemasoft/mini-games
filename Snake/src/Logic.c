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

#include "GameConfig.h"
#include "GameState.h"
#include "Input.h"
#include "Sound.h"
#include "Tools.h"

enum MoveSnake
{
  MoveSnake_Ok,
  MoveSnake_Food,
  MoveSnake_Wall,
  MoveSnake_Body,
  MoveSnake_NoSpace
};

extern GLFWwindow* g_window;

static struct Field** s_empty_fields;

static int s_turn_sound_index;

static enum MoveSnake MoveSnake();
static void PlaceFood();

static struct Field* GetField(int x, int y);
static void PlayTurnLeftSound();
static void PlayTurnRightSound();

void L_Start()
{
  srand((unsigned int) time(NULL));

  L_Restart();
}

void L_Restart()
{
  g_game_state.size.x = g_game_config.size.x;
  g_game_state.size.y = g_game_config.size.y;

  g_game_state.field_count = (size_t)(g_game_state.size.x * g_game_state.size.y);

  if (g_game_state.fields != NULL)
  {
    free(g_game_state.fields);
  }

  g_game_state.fields = malloc(g_game_state.field_count * sizeof(struct Field));

  if (g_game_state.snake != NULL)
  {
    free(g_game_state.snake);
  }

  g_game_state.snake_count = g_game_state.field_count;

  g_game_state.snake = malloc(g_game_state.snake_count * sizeof(struct Field*));

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
  struct Field* body = GetField(g_game_state.size.x / 2, g_game_state.size.y / 2 - 1);
  struct Field* tail = GetField(g_game_state.size.x / 2, g_game_state.size.y / 2 - 2);

  head->val = Value_Snake;
  body->val = Value_Snake;
  tail->val = Value_Snake;

  g_game_state.head = 0;
  g_game_state.tail = 0;
  g_game_state.snake[g_game_state.head++] = tail;
  g_game_state.snake[g_game_state.head++] = body;
  g_game_state.snake[g_game_state.head++] = head;

  g_game_state.head_offset = 0.0f;
  g_game_state.tail_offset = 0.0f;

  g_game_state.heading = Heading_Up;

  g_game_state.state = State_Play;

  g_game_state.max_move_time = 0.25;

  g_game_state.score = 0;

  if (s_empty_fields != NULL)
  {
    free(s_empty_fields);
  }

  s_empty_fields = malloc(g_game_state.field_count * sizeof(struct Field*));

  s_turn_sound_index = 0;

  PlaceFood();
}

void L_Update()
{
  static double before;
  static double move_time;

  static enum State paused_state;

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

  enum State next_state = g_game_state.state;

  switch (g_game_state.state)
  {
    case State_Play:
    case State_Play_SnakeGrowing:
      if (I_PauseKey())
      {
        paused_state = g_game_state.state;
        next_state = State_Pause;
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
              switch (g_game_state.heading)
              {
                case Heading_Left:
                case Heading_Right:
                  break;

                case Heading_Down:
                  PlayTurnLeftSound();

                  g_game_state.heading = Heading_Left;
                  break;

                case Heading_Up:
                  PlayTurnRightSound();

                  g_game_state.heading = Heading_Left;
                  break;
              }
              break;

            case GLFW_KEY_RIGHT:
              switch (g_game_state.heading)
              {
                case Heading_Left:
                case Heading_Right:
                  break;

                case Heading_Down:
                  PlayTurnRightSound();

                  g_game_state.heading = Heading_Right;
                  break;

                case Heading_Up:
                  PlayTurnLeftSound();

                  g_game_state.heading = Heading_Right;
                  break;
              }
              break;

            case GLFW_KEY_DOWN:
              switch (g_game_state.heading)
              {
                case Heading_Left:
                  PlayTurnRightSound();

                  g_game_state.heading = Heading_Down;
                  break;

                case Heading_Right:
                  PlayTurnLeftSound();

                  g_game_state.heading = Heading_Down;
                  break;

                case Heading_Down:
                case Heading_Up:
                  break;
              }
              break;

            case GLFW_KEY_UP:
              switch (g_game_state.heading)
              {
                case Heading_Left:
                  PlayTurnLeftSound();

                  g_game_state.heading = Heading_Up;
                  break;

                case Heading_Right:
                  PlayTurnRightSound();

                  g_game_state.heading = Heading_Up;
                  break;

                case Heading_Down:
                case Heading_Up:
                  break;
              }
              break;
          }

          switch (MoveSnake())
          {
            case MoveSnake_Ok:
              next_state = State_Play;
              break;

            case MoveSnake_Food:
              S_PlaySound(Sound_Food);

              ++g_game_state.score;

              PlaceFood();

              next_state = State_Play_SnakeGrowing;
              break;

            case MoveSnake_Wall:
              S_PlaySound(Sound_Wall);

              next_state = State_Fail;
              break;

            case MoveSnake_Body:
              S_PlaySound(Sound_Body);

              next_state = State_Fail;
              break;

            case MoveSnake_NoSpace:
              S_PlaySound(Sound_Success);

              next_state = State_Success;
              break;
          }
        }

        if (next_state == State_Play)
        {
          g_game_state.head_offset = (float) (move_time / g_game_state.max_move_time);
          g_game_state.tail_offset = (float) (move_time / g_game_state.max_move_time);
        }
        else if (next_state == State_Play_SnakeGrowing)
        {
          g_game_state.head_offset = (float) (move_time / g_game_state.max_move_time);
        }
        else
        {
          g_game_state.head_offset = 1.0f;
          g_game_state.tail_offset = 1.0f;
        }
      }
      break;

    case State_Success:
      break;

    case State_Fail:
      break;

    case State_Pause:
      if (I_PauseKey())
      {
        next_state = paused_state;
      }
      break;
  }

  g_game_state.state = next_state;
}

void L_Stop()
{
  free(g_game_state.fields);
  free(g_game_state.snake);
  free(s_empty_fields);
}

enum MoveSnake MoveSnake()
{
  const struct Field* head = GetSnakeHead();

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

  SetNewSnakeHead(new_head);

  if (res == MoveSnake_Food)
  {
    if (IsSnakeEmpty())
    {
      return MoveSnake_NoSpace;
    }
  }
  else
  {
    RemoveSnakeTail();
  }

  return res;
}

void PlaceFood()
{
  size_t count = 0;
  for (size_t i = 0; i < g_game_state.field_count; ++i)
  {
    if (g_game_state.fields[i].val == Value_Empty)
    {
      s_empty_fields[count++] = &g_game_state.fields[i];
    }
  }

  s_empty_fields[(size_t) rand() % count]->val = Value_Food;
}

struct Field* GetField(int x, int y)
{
  assert(x >= 0 && x < g_game_state.size.x);
  assert(y >= 0 && y < g_game_state.size.y);

  return &g_game_state.fields[x + y * g_game_state.size.x];
}

void PlayTurnLeftSound()
{
  S_PlaySound(Sound_Turn1 + s_turn_sound_index);

  s_turn_sound_index = (s_turn_sound_index + 1) % 4;
}

void PlayTurnRightSound()
{
  S_PlaySound(Sound_Turn1 + s_turn_sound_index);

  s_turn_sound_index = (s_turn_sound_index - 1 + 4) % 4;
}
