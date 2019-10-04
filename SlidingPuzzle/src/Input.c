// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Input.h"

#include <stddef.h>  // size_t

#include <GL/freeglut.h>

#define CONTROL_KEYS_SIZE 4

static bool s_reset_key;

static int s_control_keys[CONTROL_KEYS_SIZE];
static size_t s_top;
static size_t s_bot;

static bool s_control_button;
static int s_control_x;
static int s_control_y;

static void Keyboard(unsigned char key, int x, int y);
static void Special(int key, int x, int y);
static void Mouse(int button, int state, int x, int y);

void I_Start()
{
  I_Restart();

  glutKeyboardFunc(&Keyboard);
  glutSpecialFunc(&Special);
  glutMouseFunc(&Mouse);
}

void I_Update()
{
}

void I_Stop()
{
  glutKeyboardFunc(NULL);
  glutSpecialFunc(NULL);
  glutMouseFunc(NULL);
}

void I_Restart()
{
  s_reset_key = false;

  s_top = 0;
  s_bot = 0;

  s_control_button = false;
}

bool I_ResetKey()
{
  bool res = s_reset_key;

  s_reset_key = false;

  return res;
}

int I_PopControlKey()
{
  if (s_bot == s_top)
  {
    return -1;
  }

  int key = s_control_keys[s_bot++];

  if (s_bot > CONTROL_KEYS_SIZE - 1)
  {
    s_bot = 0;
  }

  return key;
}

bool I_ControlButton(int* x, int* y)
{
  bool res = s_control_button;

  s_control_button = false;

  if (res)
  {
    *x = s_control_x;
    *y = s_control_y;
  }

  return res;
}

void Keyboard(unsigned char key, int x, int y)
{
  (void) x;
  (void) y;

  // Escape
  if (key == 27)
  {
    glutLeaveMainLoop();
    return;
  }

  // r
  if (key == 114)
  {
    s_reset_key = true;
    return;
  }
}

void Special(int key, int x, int y)
{
  (void) x;
  (void) y;

  switch (key)
  {
    case GLUT_KEY_LEFT:
    case GLUT_KEY_RIGHT:
    case GLUT_KEY_DOWN:
    case GLUT_KEY_UP:
      s_control_keys[s_top++] = key;

      if (s_top > CONTROL_KEYS_SIZE - 1)
      {
        s_top = 0;
      }
      break;
  }
}

void Mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    s_control_button = true;
    s_control_x = x;
    s_control_y = y;
  }
}
