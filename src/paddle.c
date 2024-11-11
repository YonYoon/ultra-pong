#include "raylib.h"
#include "paddle.h"
#include "config.h"

Paddle paddle_setup(int x)
{
    Paddle paddle;
    // Paddles setup
    paddle.rect.width = 20;
    paddle.rect.height = 100;

    paddle.rect.x = x;
    paddle.rect.y = (SCREEN_HEIGHT / 2) - (paddle.rect.height / 2);

    paddle.speed = 5.0;
    paddle.acceleration = 1.0;

    paddle.can_dash = false;
    paddle.dash_meter = 0;

    return paddle;
}

void move_paddle_up(Paddle *paddle)
{
    paddle->rect.y -= paddle->speed * paddle->acceleration;
    if (paddle->rect.y <= 0)
        paddle->rect.y = 0;
}

void move_paddle_down(Paddle *paddle)
{
    paddle->rect.y += paddle->speed * paddle->acceleration;
    if ((paddle->rect.y + 100) >= SCREEN_HEIGHT)
        paddle->rect.y = SCREEN_HEIGHT - 100;
}

void dash_paddle(Paddle *paddle)
{
    paddle->acceleration = 3.0;
}