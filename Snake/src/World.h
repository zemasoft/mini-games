#ifndef WORLD_H
#define WORLD_H

#include <stddef.h>  // size_t

struct Field
{
  enum FieldValue
  {
    FieldValue_Empty,
    FieldValue_Food,
    FieldValue_Wall,
    FieldValue_Snake
  } value;

  struct
  {
    int x;
    int y;
  } pos;
};

struct Ground
{
  struct
  {
    int x;
    int y;
  } size;

  struct Field* fields;
  size_t field_count;
};

struct Snake
{
  enum SnakeState
  {
    SnakeState_Normal,
    SnakeState_Growing
  } state;

  struct Field** fields;
  size_t field_count;
  size_t head;
  size_t tail;

  float head_offset;
  float tail_offset;

  enum SnakeHeading
  {
    SnakeHeading_Left,
    SnakeHeading_Right,
    SnakeHeading_Down,
    SnakeHeading_Up
  } heading;
};

struct World
{
  enum WorldState
  {
    WorldState_Play,
    WorldState_Success,
    WorldState_Fail,
    WorldState_Pause
  } state;

  struct Ground ground;

  struct Snake snake;

  float max_move_time;

  int score;
};

extern struct World g_world;

#endif  // WORLD_H
