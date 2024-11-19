#include "raylib.h"
#include "ball.h"
#include "config.h"
#include "paddle.h"

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

void update_ball(Ball *ball)
{
    ball->center.x += ball->speed.x;
    ball->center.y += ball->speed.y;

    // Limit ball's speed
    if (ball->speed.x > 45.0)
    {
        ball->speed.x = 45.0;
    }
    else if (ball->speed.x < -45.0)
    {
        ball->speed.x = -45.0;
    }
}

void check_collision_ball_walls(Ball *ball, Sound collisionSFX)
{
    // Collision with screen bounds
    if (ball->center.y - ball->radius <= 0)
    {
        ball->speed.y *= -SPEED_MULTIPLIER;
        PlaySound(collisionSFX);
    }
    if (ball->center.y + ball->radius >= SCREEN_HEIGHT)
    {
        ball->speed.y *= -SPEED_MULTIPLIER;
        PlaySound(collisionSFX);
    }
}

bool is_right_player_goal(Ball *ball, Sound goalSFX)
{
    if (ball->center.x + ball->radius <= 0)
    {
        PlaySound(goalSFX);
        ball->center.x = SCREEN_WIDTH / 2;
        ball->center.y = SCREEN_HEIGHT / 2;
        ball->speed.x = -BALL_SPEED_X;
        ball->speed.y = random_speed_y();
        return true;
    }
    else
    {
        return false;
    }
}

bool is_left_player_goal(Ball *ball, Sound goalSFX)
{
    if (ball->center.x - ball->radius >= SCREEN_WIDTH)
    {
        PlaySound(goalSFX);
        ball->center.x = SCREEN_WIDTH / 2;
        ball->center.y = SCREEN_HEIGHT / 2;
        ball->speed.x = BALL_SPEED_X;
        ball->speed.y = random_speed_y();
        return true;
    }
    else
    {
        return false;
    }
}

void check_collision_ball_paddle(Ball *ball, Paddle *paddle, Sound ultraSFX, Sound collisionSFX)
{
    if (CheckCollisionCircleRec(ball->center, ball->radius, paddle->rect))
    {
        ball->speed.x *= -SPEED_MULTIPLIER;
        if (paddle->speed > 0 && ball->speed.y < 0)
        {
            ball->speed.y *= -1;
        }
        else if (paddle->speed < 0 && ball->speed.y > 0)
        {
            ball->speed.y *= -1;
        }
        if (paddle->is_ultra)
            PlaySound(ultraSFX);
        else
            PlaySound(collisionSFX);
        if (!paddle->is_ultra)
        {
            paddle->dash_meter += 1;
        }
        paddle->is_ultra = true;
    }
    else
    {
        paddle->is_ultra = false;
    }
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
