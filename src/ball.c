#include "raylib.h"
#include "ball.h"
#include "config.h"

// Ball setup
Ball ball_setup()
{
    Ball ball;
    ball.center.x = SCREEN_WIDTH / 2;
    ball.center.y = SCREEN_HEIGHT / 2;
    ball.radius = 15.0;
    ball.speed.x = BALL_SPEED_X;
    ball.speed.y = GetRandomValue(3, 6);
    return ball;
}

float random_speed_y()
{
    if (GetRandomValue(0, 1) == 0)
    {
        return (float)GetRandomValue(3, 6);
    }
    else
    {
        return (float)GetRandomValue(-3, -6);
    }
}
