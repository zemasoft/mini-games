// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Input.h"

#include <GL/freeglut.h>

static bool s_reset_key;
static bool s_control_key;
static bool s_control_button;

static void Keyboard(unsigned char key, int x, int y);
static void Mouse(int button, int state, int x, int y);

void I_Start()
{
  I_Restart();

  glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
  glutKeyboardFunc(&Keyboard);
  glutMouseFunc(&Mouse);
}

void I_Update()
{
}

void I_Stop()
{
  glutKeyboardFunc(NULL);
  glutMouseFunc(NULL);
}

void I_Restart()
{
  s_reset_key = false;
  s_control_key = false;
  s_control_button = false;
}

bool I_ResetKey()
{
  bool res = s_reset_key;

  s_reset_key = false;

  return res;
}

bool I_ControlKey()
{
  bool res = s_control_key;

  s_control_key = false;

  return res;
}

bool I_ControlButton()
{
  bool res = s_control_button;

  s_control_button = false;

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

  // Space
  if (key == 32)
  {
    s_control_key = true;
    return;
  }
}

void Mouse(int button, int state, int x, int y)
{
  (void) x;
  (void) y;

  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    s_control_button = true;
  }
}
