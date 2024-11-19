#ifndef BALL_H
#define BALL_H

#include "raylib.h"
#include "paddle.h"

typedef struct Ball
{
    Vector2 center;
    float radius;
    Vector2 speed;
} Ball;

Ball ball_setup();
void update_ball(Ball *ball);
void check_collision_ball_paddle(Ball *ball, Paddle *paddle, Sound ultraSFX, Sound collisionSFX);
bool is_left_player_goal(Ball *ball, Sound goalSFX);
bool is_right_player_goal(Ball *ball, Sound goalSFX);
void check_collision_ball_walls(Ball *ball, Sound collisionSFX);
float random_speed_y();

#endif