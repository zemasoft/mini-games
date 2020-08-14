#ifndef COMMONCORE_H
#define COMMONCORE_H

#include <stdbool.h>  // bool

bool CC_Initialize(int* argcp, char** argv);
void CC_Terminate();

bool CC_CreateWindow(int width, int height, char const* title);
void CC_DestroyWindow();

int CC_GetWindowWidth();
int CC_GetWindowHeight();
void CC_SetWindowTitle(char const* title);

typedef void (*CC_ResizeCallback)(int, int);
void CC_SetResizeCallback(CC_ResizeCallback resizeCallback);

typedef void (*CC_UpdateCallback)(unsigned);
void CC_SetUpdateCallback(CC_UpdateCallback updateCallback);

void CC_EnterMainLoop();
void CC_LeaveMainLoop();

void CC_SwapBuffers();

unsigned CC_GetElapsedTime();

int CC_LoadSound(char const* fileName);
void CC_PlaySound(int soundId);

#endif  // COMMONCORE_H
