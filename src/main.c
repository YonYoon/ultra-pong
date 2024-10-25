#include "raylib.h"

#include "resource_dir.h" // utility header for SearchAndSetResourceDir

static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 800;

typedef struct Ball
{
	Vector2 center;
	float radius;
	Vector2 speed;
} Ball;

Ball ball = {0};
Rectangle paddle_left = {0};
Rectangle paddle_right = {0};

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Eight Two Two");

	SetTargetFPS(60);

	SearchAndSetResourceDir("resources");

	int left_player_score = 0;
	int right_player_score = 0;

	// Ball setup
	ball.center.x = SCREEN_WIDTH / 2;
	ball.center.y = SCREEN_HEIGHT / 2;
	ball.radius = 15.0;
	ball.speed.x = 3.0;
	ball.speed.y = 3.0;

	const float speed_multiplier = 1.1;

	// Paddles setup
	paddle_left.width = 20;
	paddle_left.height = 100;
	paddle_right.width = 20;
	paddle_right.height = 100;

	paddle_left.x = 50;
	paddle_left.y = (SCREEN_HEIGHT / 2) - (paddle_left.height / 2);

	paddle_right.x = SCREEN_WIDTH - 70;
	paddle_right.y = (SCREEN_HEIGHT / 2) - (paddle_right.height / 2);

	while (!WindowShouldClose())
	{
		ball.center.x += ball.speed.x;
		ball.center.y += ball.speed.y;

		// Collision with screen bounds
		if (ball.center.y - ball.radius <= 0)
			ball.speed.y *= -speed_multiplier;
		if (ball.center.y + ball.radius >= SCREEN_HEIGHT)
			ball.speed.y *= -speed_multiplier;
		if (ball.center.x - ball.radius >= SCREEN_WIDTH)
		{
			left_player_score += 1;
			ball.center.x = SCREEN_WIDTH / 2;
			ball.center.y = SCREEN_HEIGHT / 2;
			ball.speed.x = 3;
			ball.speed.y = 3;
		}
		if (ball.center.x + ball.radius <= 0)
		{
			right_player_score += 1;
			ball.center.x = SCREEN_WIDTH / 2;
			ball.center.y = SCREEN_HEIGHT / 2;
			ball.speed.x = -3;
			ball.speed.y = 3;
		}

		// Collision with left and right paddle
		if (CheckCollisionCircleRec(ball.center, ball.radius, paddle_left) ||
			CheckCollisionCircleRec(ball.center, ball.radius, paddle_right))
		{
			ball.speed.x *= -speed_multiplier;
		}

		// Left paddle controls
		if (IsKeyDown(KEY_S))
			paddle_left.y += 5;
		if ((paddle_left.y + 100) >= SCREEN_HEIGHT)
			paddle_left.y = SCREEN_HEIGHT - 100;
		if (IsKeyDown(KEY_W))
			paddle_left.y -= 5;
		if (paddle_left.y <= 0)
			paddle_left.y = 0;

		// Right paddle controls
		if (IsKeyDown(KEY_DOWN))
			paddle_right.y += 5;
		if ((paddle_right.y + 100) >= SCREEN_HEIGHT)
			paddle_right.y = SCREEN_HEIGHT - 100;
		if (IsKeyDown(KEY_UP))
			paddle_right.y -= 5;
		if (paddle_right.y <= 0)
			paddle_right.y = 0;

		BeginDrawing();

		ClearBackground(BLACK);

		DrawText("822", 200, 200, 20, WHITE);

		DrawRectangleRec(paddle_left, WHITE);
		DrawRectangleRec(paddle_right, WHITE);
		DrawCircleV(ball.center, ball.radius, WHITE);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}