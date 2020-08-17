#ifndef ZGE_H
#define ZGE_H

#include <stdbool.h>  // bool

bool zgeInitialize(int* argcp, char** argv);
void zgeTerminate();

bool zgeCreateWindow(int width, int height, char const* title);
void zgeDestroyWindow();

int zgeGetWindowWidth();
int zgeGetWindowHeight();
void zgeSetWindowTitle(char const* title);

typedef void (*zgeResizeCallback)(int, int);
void zgeSetResizeCallback(zgeResizeCallback resizeCallback);

typedef void (*zgeUpdateCallback)(unsigned);
void zgeSetUpdateCallback(zgeUpdateCallback updateCallback);

void zgeEnterMainLoop();
void zgeLeaveMainLoop();

void zgeSwapBuffers();

unsigned zgeGetElapsedTime();

int zgeLoadSound(char const* fileName);
void zgePlaySound(int soundId);

#endif  // ZGE_H
