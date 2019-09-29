// Copyright Tomas Zeman 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Input.h"

#include <stdbool.h>  // true

#include <GL/freeglut.h>

#include "GameState.h"

static void Keyboard(unsigned char key, int x, int y);
static void Mouse(int button, int state, int x, int y);

void I_Start()
{
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

  // Space
  if (key == 32)
  {
    g_game_state.control_key = true;
    return;
  }

  // r
  if (key == 114)
  {
    g_game_state.reset_key = true;
    return;
  }
}

void Mouse(int button, int state, int x, int y)
{
  (void) x;
  (void) y;

  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    g_game_state.mouse_button = true;
  }
}
