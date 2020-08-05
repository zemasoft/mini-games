#ifndef COMMON_CORE_H
#define COMMON_CORE_H

#include <stdbool.h>  // bool

bool CC_Initialize(int argc, char** argv);
void CC_Terminate();

unsigned CC_GetElapsedTime();

#endif  // COMMON_CORE_H
