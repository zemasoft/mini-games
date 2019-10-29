// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Tools.h"

#include <assert.h>  // assert

#include "GameState.h"

bool IsSnakeEmpty()
{
  return g_game_state.head == g_game_state.tail;
}

size_t GetSnakeLength()
{
  return (g_game_state.head - g_game_state.tail + g_game_state.snake_count) %
         g_game_state.snake_count;
}

void SetNewSnakeHead(struct Field* new_head)
{
  assert(new_head->val == Value_Snake);

  g_game_state.snake[g_game_state.head] = new_head;
  g_game_state.head = GetNextSnakeIndex(g_game_state.head);
}

struct Field* GetSnakeHead()
{
  struct Field* head = g_game_state.snake[GetSnakeHeadIndex()];

  assert(head->val == Value_Snake);

  return head;
}

void RemoveSnakeTail()
{
  GetSnakeTail()->val = Value_Empty;
  g_game_state.tail = GetNextSnakeIndex(g_game_state.tail);
}

struct Field* GetSnakeTail()
{
  struct Field* tail = g_game_state.snake[GetSnakeTailIndex()];

  assert(tail->val == Value_Snake);

  return tail;
}

struct Field* GetSnakeElement(size_t index)
{
  assert(!IsSnakeEmpty());

  if (g_game_state.head > g_game_state.tail)
  {
    if (index < g_game_state.tail || index >= g_game_state.head)
    {
      return NULL;
    }
  }
  else
  {
    if (index >= g_game_state.head && index < g_game_state.tail)
    {
      return NULL;
    }
  }

  struct Field* element = g_game_state.snake[index];

  assert(element->val == Value_Snake);

  return element;
}

struct Field* GetNextSnakeElement(size_t index)
{
  return GetSnakeElement(GetNextSnakeIndex(index));
}

struct Field* GetPrevSnakeElement(size_t index)
{
  return GetSnakeElement(GetPrevSnakeIndex(index));
}

size_t GetSnakeHeadIndex()
{
  assert(!IsSnakeEmpty());

  return GetPrevSnakeIndex(g_game_state.head);
}

size_t GetSnakeTailIndex()
{
  assert(!IsSnakeEmpty());

  return g_game_state.tail;
}

size_t GetNextSnakeIndex(size_t index)
{
  return (index + 1) % g_game_state.snake_count;
}

size_t GetPrevSnakeIndex(size_t index)
{
  return (index - 1 + g_game_state.snake_count) % g_game_state.snake_count;
}
