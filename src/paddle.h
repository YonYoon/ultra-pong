#ifndef PADDLE_H
#define PADDLE_H

#include "raylib.h"

typedef struct Paddle
{
    Rectangle rect;
    Texture2D tex;
    float speed;
    float acceleration;
    int number_of_hits;
    int dash_charges;
    bool is_ultra;
} Paddle;

Paddle paddle_setup(int x, Texture2D tex);
void move_paddle_up(Paddle *paddle);
void move_paddle_down(Paddle *paddle);
void dash_paddle(Paddle *paddle);
void reset_paddles(Paddle *paddle_left, Paddle *paddle_right);

#endif