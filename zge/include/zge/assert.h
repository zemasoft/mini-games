#ifndef ZGE_ASSERT_H
#define ZGE_ASSERT_H

void _ZGE_Abort(char const* fileName, unsigned lineNumber, char const* failedAssertion);

#define ZGE_Assert(expr) ((expr) ? (void) 0 : _ZGE_Abort(__FILE__, __LINE__, #expr))

#ifdef NDEBUG
#define ZGE_AssertDebug(expr) ((void) (0 && (expr)))
#else
#define ZGE_AssertDebug ZGE_Assert
#endif

#endif  // ZGE_ASSERT_H
