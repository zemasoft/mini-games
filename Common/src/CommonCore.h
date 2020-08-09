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

typedef void (*CC_DisplayFnc)(void);
void CC_SetDisplayFnc(CC_DisplayFnc displayFnc);

typedef void (*CC_ResizeFnc)(int, int);
void CC_SetResizeFnc(CC_ResizeFnc resizeFnc);

typedef void (*CC_UpdateFnc)(void);
void CC_SetUpdateFnc(CC_UpdateFnc updateFnc);

void CC_EnterMainLoop();
void CC_LeaveMainLoop();

void CC_SwapBuffers();

unsigned CC_GetElapsedTime();

#endif  // COMMONCORE_H
