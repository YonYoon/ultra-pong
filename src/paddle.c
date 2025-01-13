#include "raylib.h"
#include "paddle.h"
#include "config.h"

const float PADDLE_SPEED = 7.0;

Paddle paddle_setup(int x, Texture2D tex)
{
    Paddle paddle;

    paddle.rect.width = 20;
    paddle.rect.height = 100;

    paddle.rect.x = x;
    paddle.rect.y = (SCREEN_HEIGHT / 2) - (paddle.rect.height / 2);

    paddle.tex = tex;

    paddle.speed = PADDLE_SPEED;
    paddle.acceleration = 1.0;

    paddle.number_of_hits = 0;

    paddle.dash_charges = 0;

    paddle.is_ultra = false;

    return paddle;
}

void move_paddle_up(Paddle *paddle)
{
    paddle->speed = -PADDLE_SPEED;
    paddle->rect.y += paddle->speed * paddle->acceleration;
    if (paddle->rect.y <= 0)
        paddle->rect.y = 0;
}

void move_paddle_down(Paddle *paddle)
{
    paddle->speed = PADDLE_SPEED;
    paddle->rect.y += paddle->speed * paddle->acceleration;
    if ((paddle->rect.y + 100) >= SCREEN_HEIGHT)
        paddle->rect.y = SCREEN_HEIGHT - 100;
}

void dash_paddle(Paddle *paddle)
{
    paddle->acceleration = 4.0;
    paddle->dash_charges -= 1;
}

void reset_paddles(Paddle *paddle_left, Paddle *paddle_right)
{
    paddle_left->rect.x = 50;
    paddle_left->rect.y = (SCREEN_HEIGHT / 2) - (paddle_left->rect.height / 2);
    paddle_left->dash_charges = 0;

    paddle_right->rect.x = SCREEN_WIDTH - 70;
    paddle_right->rect.y = (SCREEN_HEIGHT / 2) - (paddle_right->rect.height / 2);
    paddle_right->dash_charges = 0;
}