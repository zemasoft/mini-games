#ifndef ZGE_CORE_H
#define ZGE_CORE_H

#include <stdbool.h>  // bool

bool ZGE_Initialize(int* argcp, char** argv);
void ZGE_Terminate();

bool ZGE_CreateWindow(int width, int height, char const* title);
void ZGE_DestroyWindow();

int ZGE_GetWindowWidth();
int ZGE_GetWindowHeight();
void ZGE_SetWindowTitle(char const* title);

typedef void (*ZGE_ResizeCallback)(int, int);
void ZGE_SetResizeCallback(ZGE_ResizeCallback resizeCallback);

typedef void (*ZGE_UpdateCallback)(unsigned);
void ZGE_SetUpdateCallback(ZGE_UpdateCallback updateCallback);

void ZGE_EnterGameLoop();
void ZGE_LeaveGameLoop();

void ZGE_SwapBuffers();

unsigned ZGE_GetElapsedTime();

#endif  // ZGE_CORE_H
