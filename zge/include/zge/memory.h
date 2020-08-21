#ifndef ZGE_MEMORY_H
#define ZGE_MEMORY_H

#include <stddef.h>  // size_t

void* ZGE_Allocate(size_t size);
void* ZGE_Reallocate(void* ptr, size_t size);
void ZGE_Free(void* ptr);
void ZGE_FreeIfAllocated(void* ptr);

#endif  // ZGE_MEMORY_H
