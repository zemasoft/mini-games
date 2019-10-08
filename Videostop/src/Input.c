// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Input.h"

#include <GL/freeglut.h>

static bool s_reset_key;
static bool s_control_key;
static bool s_control_button;
static bool s_size_up_key;
static bool s_size_down_key;
static bool s_speed_up_key;
static bool s_speed_down_key;

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
  s_size_up_key = false;
  s_size_down_key = false;
  s_speed_up_key = false;
  s_speed_down_key = false;
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

bool I_SizeUpKey()
{
  bool res = s_size_up_key;

  s_size_up_key = false;

  return res;
}

bool I_SizeDownKey()
{
  bool res = s_size_down_key;

  s_size_down_key = false;

  return res;
}

bool I_SpeedUpKey()
{
  bool res = s_speed_up_key;

  s_speed_up_key = false;

  return res;
}

bool I_SpeedDownKey()
{
  bool res = s_speed_down_key;

  s_speed_down_key = false;

  return res;
}

void Keyboard(unsigned char key, int x, int y)
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

void Special(int key, int x, int y)
{
  (void) x;
  (void) y;

  switch (key)
  {
    case GLUT_KEY_RIGHT:
      s_size_up_key = true;
      break;
    case GLUT_KEY_LEFT:
      s_size_down_key = true;
      break;
    case GLUT_KEY_UP:
      s_speed_up_key = true;
      break;
    case GLUT_KEY_DOWN:
      s_speed_down_key = true;
      break;
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
