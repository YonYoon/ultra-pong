#ifndef PADDLE_H
#define PADDLE_H

#include "raylib.h"

typedef struct Paddle
{
    Rectangle rect;
    float speed;
    float acceleration;
    int dash_meter;
    bool is_ultra;
} Paddle;

Paddle paddle_setup(int x);
void move_paddle_up(Paddle *paddle);
void move_paddle_down(Paddle *paddle);
void dash_paddle(Paddle *paddle);

#endif