#ifndef ZGE_ASSERT_H
#define ZGE_ASSERT_H

#ifdef NDEBUG
#undef NDEBUG
#include <assert.h>  // assert
#define NDEBUG
#else
#include <assert.h>  // assert
#endif

#define ZGE_Assert(expr) assert(expr)

#ifdef NDEBUG
#define ZGE_AssertDebug(expr) ((void) (0 && (expr)))
#else
#define ZGE_AssertDebug ZGE_Assert
#endif

#endif  // ZGE_ASSERT_H
