#include "Logic.h"

#include <assert.h>  // assert
#include <stddef.h>  // size_t
#include <stdlib.h>  // free, malloc, srand, rand
#include <time.h>    // time

#if defined(USE_FREEGLUT)
#include <GL/freeglut.h>
#endif

#if defined(USE_GLFW)
#include <GLFW/glfw3.h>
#endif

#if defined(USE_SDL2)
#include <SDL2/SDL.h>
#endif

#include "Config.h"
#include "Input.h"
#include "Sound.h"
#include "Tools.h"
#include "World.h"

enum MoveSnake
{
  MoveSnake_Ok,
  MoveSnake_Food,
  MoveSnake_Wall,
  MoveSnake_Body,
  MoveSnake_NoSpace
};

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
  g_world.state = WorldState_Pause;

  g_world.ground.size.x = g_config.ground.size.x;
  g_world.ground.size.y = g_config.ground.size.y;

  g_world.ground.field_count = (size_t)(g_world.ground.size.x * g_world.ground.size.y);

  if (g_world.ground.fields != NULL)
  {
    free(g_world.ground.fields);
  }

  g_world.ground.fields = malloc(g_world.ground.field_count * sizeof(struct Field));

  for (int y = 0; y < g_world.ground.size.y; ++y)
  {
    for (int x = 0; x < g_world.ground.size.x; ++x)
    {
      struct Field* const field = GetField(x, y);

      field->value = FieldValue_Empty;
      field->pos.x = x;
      field->pos.y = y;
    }
  }

  g_world.snake.state = SnakeState_Normal;

  if (g_world.snake.fields != NULL)
  {
    free(g_world.snake.fields);
  }

  g_world.snake.field_count = g_world.ground.field_count;

  g_world.snake.fields = malloc(g_world.snake.field_count * sizeof(struct Field*));

  int const init_y_pos = g_world.ground.size.y / 2;

  struct Field* const head = GetField(4, init_y_pos);
  struct Field* const body = GetField(3, init_y_pos);
  struct Field* const tail = GetField(2, init_y_pos);

  head->value = FieldValue_Snake;
  body->value = FieldValue_Snake;
  tail->value = FieldValue_Snake;

  GetField(g_world.ground.size.x - 4, init_y_pos)->value = FieldValue_Food;

  g_world.snake.head = 0;
  g_world.snake.tail = 0;
  g_world.snake.fields[g_world.snake.head++] = tail;
  g_world.snake.fields[g_world.snake.head++] = body;
  g_world.snake.fields[g_world.snake.head++] = head;

  g_world.snake.head_offset = 0.0f;
  g_world.snake.tail_offset = 0.0f;

  g_world.snake.heading = SnakeHeading_Right;

  g_world.max_move_time = 250.0f;

  g_world.score = 0;

  if (s_empty_fields != NULL)
  {
    free(s_empty_fields);
  }

  s_empty_fields = malloc(g_world.ground.field_count * sizeof(struct Field*));

  s_turn_sound_index = 0;
}

void L_Update()
{
  static float move_time;

  if (I_ResetKey())
  {
    I_Restart();
    L_Restart();
    S_Restart();
    // G_Restart();

    move_time = 0.0f;
    return;
  }

  switch (g_world.state)
  {
    case WorldState_Play:
      if (I_PauseKey())
      {
        g_world.state = WorldState_Pause;
      }
      else
      {
        move_time += UPDATE_PERIOD_MS;

        if (move_time >= g_world.max_move_time)
        {
          move_time = 0.0f;

          switch (I_PopDirectionKey())
          {
            case KEY_LEFT:
              switch (g_world.snake.heading)
              {
                case SnakeHeading_Left:
                case SnakeHeading_Right:
                  break;

                case SnakeHeading_Down:
                  PlayTurnLeftSound();

                  g_world.snake.heading = SnakeHeading_Left;
                  break;

                case SnakeHeading_Up:
                  PlayTurnRightSound();

                  g_world.snake.heading = SnakeHeading_Left;
                  break;
              }
              break;

            case KEY_RIGHT:
              switch (g_world.snake.heading)
              {
                case SnakeHeading_Left:
                case SnakeHeading_Right:
                  break;

                case SnakeHeading_Down:
                  PlayTurnRightSound();

                  g_world.snake.heading = SnakeHeading_Right;
                  break;

                case SnakeHeading_Up:
                  PlayTurnLeftSound();

                  g_world.snake.heading = SnakeHeading_Right;
                  break;
              }
              break;

            case KEY_DOWN:
              switch (g_world.snake.heading)
              {
                case SnakeHeading_Left:
                  PlayTurnRightSound();

                  g_world.snake.heading = SnakeHeading_Down;
                  break;

                case SnakeHeading_Right:
                  PlayTurnLeftSound();

                  g_world.snake.heading = SnakeHeading_Down;
                  break;

                case SnakeHeading_Down:
                case SnakeHeading_Up:
                  break;
              }
              break;

            case KEY_UP:
              switch (g_world.snake.heading)
              {
                case SnakeHeading_Left:
                  PlayTurnLeftSound();

                  g_world.snake.heading = SnakeHeading_Up;
                  break;

                case SnakeHeading_Right:
                  PlayTurnRightSound();

                  g_world.snake.heading = SnakeHeading_Up;
                  break;

                case SnakeHeading_Down:
                case SnakeHeading_Up:
                  break;
              }
              break;
          }

          switch (MoveSnake())
          {
            case MoveSnake_Ok:
              g_world.snake.state = SnakeState_Normal;
              break;

            case MoveSnake_Food:
              S_PlaySound(Sound_Food);

              ++g_world.score;

              PlaceFood();

              g_world.snake.state = SnakeState_Growing;
              break;

            case MoveSnake_Wall:
              S_PlaySound(Sound_Wall);

              g_world.state = WorldState_Fail;
              break;

            case MoveSnake_Body:
              S_PlaySound(Sound_Body);

              g_world.state = WorldState_Fail;
              break;

            case MoveSnake_NoSpace:
              S_PlaySound(Sound_Success);

              g_world.state = WorldState_Success;
              break;
          }
        }

        if (g_world.state == WorldState_Play)
        {
          switch (g_world.snake.state)
          {
            case SnakeState_Normal:
              g_world.snake.head_offset = move_time / g_world.max_move_time;
              g_world.snake.tail_offset = move_time / g_world.max_move_time;
              break;

            case SnakeState_Growing:
              g_world.snake.head_offset = move_time / g_world.max_move_time;
              break;
          }
        }
        else
        {
          g_world.snake.head_offset = 1.0f;
          g_world.snake.tail_offset = 1.0f;
        }
      }
      break;

    case WorldState_Success:
      break;

    case WorldState_Fail:
      break;

    case WorldState_Pause:
      if (I_PauseKey() || I_GetDirectionKey() != -1)
      {
        g_world.state = WorldState_Play;
      }
      break;
  }
}

void L_Stop()
{
  free(g_world.ground.fields);
  free(g_world.snake.fields);
  free(s_empty_fields);
}

enum MoveSnake MoveSnake()
{
  struct Field const* const head = GetSnakeHead();

  int new_x_pos = head->pos.x;
  int new_y_pos = head->pos.y;

  switch (g_world.snake.heading)
  {
    case SnakeHeading_Left:
      if (head->pos.x <= 0)
      {
        return MoveSnake_Wall;
      }

      --new_x_pos;
      break;

    case SnakeHeading_Right:
      if (head->pos.x >= g_world.ground.size.x - 1)
      {
        return MoveSnake_Wall;
      }

      ++new_x_pos;
      break;

    case SnakeHeading_Down:
      if (head->pos.y <= 0)
      {
        return MoveSnake_Wall;
      }

      --new_y_pos;
      break;

    case SnakeHeading_Up:
      if (head->pos.y >= g_world.ground.size.y - 1)
      {
        return MoveSnake_Wall;
      }

      ++new_y_pos;
      break;
  }

  enum MoveSnake res = MoveSnake_Ok;

  struct Field* const new_head = GetField(new_x_pos, new_y_pos);

  switch (new_head->value)
  {
    case FieldValue_Empty:
      break;

    case FieldValue_Food:
      res = MoveSnake_Food;
      break;

    case FieldValue_Wall:
      return MoveSnake_Wall;

    case FieldValue_Snake:
      return MoveSnake_Body;
  }

  new_head->value = FieldValue_Snake;

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
  for (size_t i = 0; i < g_world.ground.field_count; ++i)
  {
    if (g_world.ground.fields[i].value == FieldValue_Empty)
    {
      s_empty_fields[count++] = &g_world.ground.fields[i];
    }
  }

  s_empty_fields[(size_t) rand() % count]->value = FieldValue_Food;
}

struct Field* GetField(int const x, int const y)
{
  assert(x >= 0 && x < g_world.ground.size.x);
  assert(y >= 0 && y < g_world.ground.size.y);

  return &g_world.ground.fields[x + y * g_world.ground.size.x];
}

void PlayTurnLeftSound()
{
  S_PlaySound(Sound_Turn1 + (enum Sound) s_turn_sound_index);

  s_turn_sound_index = (s_turn_sound_index + 1) % 4;
}

void PlayTurnRightSound()
{
  S_PlaySound(Sound_Turn1 + (enum Sound) s_turn_sound_index);

  s_turn_sound_index = (s_turn_sound_index - 1 + 4) % 4;
}
