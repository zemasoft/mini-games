#include "zge/memory.h"

#include <stddef.h>  // NULL
#include <stdlib.h>  // free, malloc, realloc

#include "zge/assert.h"

void* ZGE_Allocate(size_t const size)
{
  ZGE_AssertDebug(size > 0);

  void* const ptr = malloc(size);
  ZGE_Assert(ptr != NULL);

  return ptr;
}

void* ZGE_Reallocate(void* const ptr, size_t const size)
{
  ZGE_AssertDebug(ptr != NULL);
  ZGE_AssertDebug(size > 0);

  void* const newPtr = realloc(ptr, size);
  ZGE_Assert(newPtr != NULL);

  return newPtr;
}

void ZGE_Free(void* const ptr)
{
  ZGE_AssertDebug(ptr != NULL);

  free(ptr);
}

void ZGE_FreeIfAllocated(void* const ptr)
{
  if (ptr != NULL)
  {
    free(ptr);
  }
}
