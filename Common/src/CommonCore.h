#ifndef COMMON_CORE_H
#define COMMON_CORE_H

#include <stdbool.h>  // bool

bool CC_Initialize(int argc, char** argv);
void CC_Terminate();

bool CC_CreateWindow(int width, int height, char const* title);
void CC_DestroyWindow();

unsigned CC_GetElapsedTime();

#endif  // COMMON_CORE_H
