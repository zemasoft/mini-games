// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Input.h"

#include <stddef.h>  // size_t

#include <GL/freeglut.h>

#include "Config.h"

static bool s_reset_key;
static bool s_control_key;
static bool s_control_button;
static int s_control_x;
static int s_control_y;

static int s_direction_keys[DIRECTION_KEYS_SIZE];
static size_t s_top;
static size_t s_bot;

static void Keyboard(unsigned char key, int x, int y);
static void Special(int key, int x, int y);
static void Mouse(int button, int state, int x, int y);

void I_Start()
{
  glutKeyboardFunc(&Keyboard);
  glutSpecialFunc(&Special);
  glutMouseFunc(&Mouse);

  I_Restart();
}

void I_Restart()
{
  s_reset_key = false;
  s_control_key = false;
  s_control_button = false;

  s_top = 0;
  s_bot = 0;
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

bool I_ResetKey()
{
  const bool res = s_reset_key;

  s_reset_key = false;

  return res;
}

bool I_ControlKey()
{
  const bool res = s_control_key;

  s_control_key = false;

  return res;
}

bool I_ControlButton(int* const x, int* const y)
{
  const bool res = s_control_button;

  s_control_button = false;

  if (res)
  {
    *x = s_control_x;
    *y = s_control_y;
  }

  return res;
}

int I_PopDirectionKey()
{
  if (s_bot == s_top)
  {
    return -1;
  }

  const int key = s_direction_keys[s_bot++];

  if (s_bot > DIRECTION_KEYS_SIZE - 1)
  {
    s_bot = 0;
  }

  return key;
}

void Keyboard(const unsigned char key, const int x, const int y)
{
  (void) x;
  (void) y;

  switch (key)
  {
    case 13:  // Enter
      s_control_key = true;
      break;
    case 27:  // Escape
      glutLeaveMainLoop();
      break;
    case 32:  // Space
      s_control_key = true;
      break;
    case 114:  // r
      s_reset_key = true;
      break;
  }
}

void Special(const int key, const int x, const int y)
{
  (void) x;
  (void) y;

  switch (key)
  {
    case GLUT_KEY_LEFT:
    case GLUT_KEY_RIGHT:
    case GLUT_KEY_DOWN:
    case GLUT_KEY_UP:
      s_direction_keys[s_top++] = key;

      if (s_top > DIRECTION_KEYS_SIZE - 1)
      {
        s_top = 0;
      }
      break;
  }
}

void Mouse(const int button, const int state, const int x, const int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    s_control_button = true;
    s_control_x = x;
    s_control_y = y;
  }
}
