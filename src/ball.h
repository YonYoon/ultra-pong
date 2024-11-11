#ifndef BALL_H
#define BALL_H

#include "raylib.h"

typedef struct Ball
{
    Vector2 center;
    float radius;
    Vector2 speed;
} Ball;

Ball ball_setup();

#endif