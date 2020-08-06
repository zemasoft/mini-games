#ifndef COMMON_CORE_H
#define COMMON_CORE_H

#include <stdbool.h>  // bool

bool CC_Initialize(int argc, char** argv);
void CC_Terminate();

bool CC_CreateWindow(int width, int height, char const* title);
void CC_DestroyWindow();

typedef void (*CC_ResizeFnc)(int, int);
void CC_SetResizeFnc(CC_ResizeFnc resizeFnc);

typedef void (*CC_UpdateFnc)(void);
void CC_SetUpdateFnc(CC_UpdateFnc updateFnc);

void CC_EnterMainLoop();
void CC_LeaveMainLoop();

unsigned CC_GetElapsedTime();

#endif  // COMMON_CORE_H
