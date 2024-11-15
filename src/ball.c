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
    ball.speed.x = 4.0;
    ball.speed.y = 3.0;
    return ball;
}
