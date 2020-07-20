#include "Tools.h"

#include <assert.h>  // assert

#include "World.h"

bool IsSnakeEmpty()
{
  return g_world.snake.head == g_world.snake.tail;
}

size_t GetSnakeLength()
{
  return (g_world.snake.head - g_world.snake.tail + g_world.snake.field_count) %
         g_world.snake.field_count;
}

void SetNewSnakeHead(struct Field* const new_head)
{
  assert(new_head->value == FieldValue_Snake);

  g_world.snake.fields[g_world.snake.head] = new_head;
  g_world.snake.head = GetNextSnakeIndex(g_world.snake.head);
}

struct Field* GetSnakeHead()
{
  struct Field* const head = g_world.snake.fields[GetSnakeHeadIndex()];

  assert(head->value == FieldValue_Snake);

  return head;
}

void RemoveSnakeTail()
{
  GetSnakeTail()->value = FieldValue_Empty;
  g_world.snake.tail = GetNextSnakeIndex(g_world.snake.tail);
}

struct Field* GetSnakeTail()
{
  struct Field* const tail = g_world.snake.fields[GetSnakeTailIndex()];

  assert(tail->value == FieldValue_Snake);

  return tail;
}

struct Field* GetSnakeElement(size_t const index)
{
  assert(!IsSnakeEmpty());

  if (g_world.snake.head > g_world.snake.tail)
  {
    if (index < g_world.snake.tail || index >= g_world.snake.head)
    {
      return NULL;
    }
  }
  else
  {
    if (index >= g_world.snake.head && index < g_world.snake.tail)
    {
      return NULL;
    }
  }

  struct Field* const element = g_world.snake.fields[index];

  assert(element->value == FieldValue_Snake);

  return element;
}

struct Field* GetNextSnakeElement(size_t const index)
{
  return GetSnakeElement(GetNextSnakeIndex(index));
}

struct Field* GetPrevSnakeElement(size_t const index)
{
  return GetSnakeElement(GetPrevSnakeIndex(index));
}

size_t GetSnakeHeadIndex()
{
  assert(!IsSnakeEmpty());

  return GetPrevSnakeIndex(g_world.snake.head);
}

size_t GetSnakeTailIndex()
{
  assert(!IsSnakeEmpty());

  return g_world.snake.tail;
}

size_t GetNextSnakeIndex(size_t const index)
{
  return (index + 1) % g_world.snake.field_count;
}

size_t GetPrevSnakeIndex(size_t const index)
{
  return (index - 1 + g_world.snake.field_count) % g_world.snake.field_count;
}
