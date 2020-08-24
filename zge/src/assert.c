#include "zge/assert.h"

#include <stdio.h>   // fprintf, stderr
#include <stdlib.h>  // abort

void _ZGE_Abort(char const* const fileName, unsigned const lineNumber,
                char const* const failedAssertion)
{
  fprintf(stderr, "%s:%d Assertion `%s' failed.\n", fileName, lineNumber, failedAssertion);
  abort();
}
