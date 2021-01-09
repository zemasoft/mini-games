#include "Logic.h"

#include <stddef.h>  // NULL, size_t
#include <stdlib.h>  // srand, rand
#include <time.h>    // time

#include "zge/assert.h"
#include "zge/memory.h"

#include "Audio.h"
#include "Config.h"
#include "Input.h"
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

static struct Field** s_emptyFields;

static int s_turnSoundIndex;

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

  g_world.ground.fieldCount = (size_t)(g_world.ground.size.x * g_world.ground.size.y);

  ZGE_FreeIfAllocated(g_world.ground.fields);
  g_world.ground.fields = ZGE_Allocate(g_world.ground.fieldCount * sizeof(struct Field));

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

  g_world.snake.fieldCount = g_world.ground.fieldCount;
  ZGE_FreeIfAllocated(g_world.snake.fields);
  g_world.snake.fields = ZGE_Allocate(g_world.snake.fieldCount * sizeof(struct Field*));

  int const initYPos = g_world.ground.size.y / 2;

  struct Field* const head = GetField(4, initYPos);
  struct Field* const body = GetField(3, initYPos);
  struct Field* const tail = GetField(2, initYPos);

  head->value = FieldValue_Snake;
  body->value = FieldValue_Snake;
  tail->value = FieldValue_Snake;

  GetField(g_world.ground.size.x - 4, initYPos)->value = FieldValue_Food;

  g_world.snake.head = 0;
  g_world.snake.tail = 0;
  g_world.snake.fields[g_world.snake.head++] = tail;
  g_world.snake.fields[g_world.snake.head++] = body;
  g_world.snake.fields[g_world.snake.head++] = head;

  g_world.snake.headOffset = 0.0f;
  g_world.snake.tailOffset = 0.0f;

  g_world.snake.heading = SnakeHeading_Right;

  g_world.maxMoveTime = 250.0f;

  g_world.score = 0;

  ZGE_FreeIfAllocated(s_emptyFields);
  s_emptyFields = ZGE_Allocate(g_world.ground.fieldCount * sizeof(struct Field*));

  s_turnSoundIndex = 0;
}

void L_Update()
{
  static float moveTime;

  if (I_ResetKey())
  {
    I_Restart();
    L_Restart();
    A_Restart();
    // G_Restart();

    moveTime = 0.0f;
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
        moveTime += UPDATE_PERIOD_MS;

        if (moveTime >= g_world.maxMoveTime)
        {
          moveTime = 0.0f;

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
              A_PlaySound(Sound_Food);

              ++g_world.score;

              PlaceFood();

              g_world.snake.state = SnakeState_Growing;
              break;

            case MoveSnake_Wall:
              A_PlaySound(Sound_Wall);

              g_world.state = WorldState_Fail;
              break;

            case MoveSnake_Body:
              A_PlaySound(Sound_Body);

              g_world.state = WorldState_Fail;
              break;

            case MoveSnake_NoSpace:
              A_PlaySound(Sound_Success);

              g_world.state = WorldState_Success;
              break;
          }
        }

        if (g_world.state == WorldState_Play)
        {
          switch (g_world.snake.state)
          {
            case SnakeState_Normal:
              g_world.snake.headOffset = moveTime / g_world.maxMoveTime;
              g_world.snake.tailOffset = moveTime / g_world.maxMoveTime;
              break;

            case SnakeState_Growing:
              g_world.snake.headOffset = moveTime / g_world.maxMoveTime;
              break;
          }
        }
        else
        {
          g_world.snake.headOffset = 1.0f;
          g_world.snake.tailOffset = 1.0f;
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
  ZGE_Free(g_world.ground.fields);
  ZGE_Free(g_world.snake.fields);
  ZGE_Free(s_emptyFields);
}

enum MoveSnake MoveSnake()
{
  struct Field const* const head = GetSnakeHead();

  int newXPos = head->pos.x;
  int newYPos = head->pos.y;

  switch (g_world.snake.heading)
  {
    case SnakeHeading_Left:
      if (head->pos.x <= 0)
      {
        return MoveSnake_Wall;
      }

      --newXPos;
      break;

    case SnakeHeading_Right:
      if (head->pos.x >= g_world.ground.size.x - 1)
      {
        return MoveSnake_Wall;
      }

      ++newXPos;
      break;

    case SnakeHeading_Down:
      if (head->pos.y <= 0)
      {
        return MoveSnake_Wall;
      }

      --newYPos;
      break;

    case SnakeHeading_Up:
      if (head->pos.y >= g_world.ground.size.y - 1)
      {
        return MoveSnake_Wall;
      }

      ++newYPos;
      break;
  }

  enum MoveSnake res = MoveSnake_Ok;

  struct Field* const newHead = GetField(newXPos, newYPos);

  switch (newHead->value)
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

  newHead->value = FieldValue_Snake;

  SetNewSnakeHead(newHead);

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
  for (size_t i = 0; i < g_world.ground.fieldCount; ++i)
  {
    if (g_world.ground.fields[i].value == FieldValue_Empty)
    {
      s_emptyFields[count++] = &g_world.ground.fields[i];
    }
  }

  s_emptyFields[(size_t) rand() % count]->value = FieldValue_Food;
}

struct Field* GetField(int const x, int const y)
{
  ZGE_AssertDebug(x >= 0 && x < g_world.ground.size.x);
  ZGE_AssertDebug(y >= 0 && y < g_world.ground.size.y);

  return &g_world.ground.fields[x + y * g_world.ground.size.x];
}

void PlayTurnLeftSound()
{
  A_PlaySound(Sound_Turn1 + (enum Sound) s_turnSoundIndex);

  s_turnSoundIndex = (s_turnSoundIndex + 1) % 4;
}

void PlayTurnRightSound()
{
  A_PlaySound(Sound_Turn1 + (enum Sound) s_turnSoundIndex);

  s_turnSoundIndex = (s_turnSoundIndex - 1 + 4) % 4;
}
