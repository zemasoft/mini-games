// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef TOOLS_H
#define TOOLS_H

#include <stdbool.h>  // bool
#include <stddef.h>   // size_t

struct Field;

bool IsSnakeEmpty();
size_t GetSnakeLength();

void SetNewSnakeHead(struct Field* new_head);
struct Field* GetSnakeHead();

void RemoveSnakeTail();
struct Field* GetSnakeTail();

struct Field* GetSnakeElement(size_t index);
struct Field* GetNextSnakeElement(size_t index);
struct Field* GetPrevSnakeElement(size_t index);

size_t GetSnakeHeadIndex();
size_t GetSnakeTailIndex();

size_t GetNextSnakeIndex(size_t index);
size_t GetPrevSnakeIndex(size_t index);

#endif  // TOOLS_H
