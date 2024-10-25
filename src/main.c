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

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Eight Two Two");

	SetTargetFPS(60);

	SearchAndSetResourceDir("resources");

	int left_player_score = 0;
	int right_player_score = 0;

	int left_paddle_y = 20;
	int right_paddle_y = 20;

	// Ball setup
	Ball ball = {0};
	ball.center.x = SCREEN_WIDTH / 2;
	ball.center.y = SCREEN_HEIGHT / 2;
	ball.radius = 15.0;
	ball.speed.x = 3.0;
	ball.speed.y = 3.0;

	const float speed_multiplier = 1.1;

	while (!WindowShouldClose())
	{
		ball.center.x += ball.speed.x;
		ball.center.y += ball.speed.y;

		// Collision with screen bounds
		if (ball.center.y - ball.radius <= 0) ball.speed.y *= -speed_multiplier;
		if (ball.center.y + ball.radius >= SCREEN_HEIGHT) ball.speed.y *= -speed_multiplier;
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

		// Collision with left paddle
		if (CheckCollisionCircleRec(ball.center, ball.radius, (Rectangle){50, left_paddle_y, 20, 100}))
		{
			ball.speed.x *= -speed_multiplier;
		}

		// Collision with right paddle
		if (CheckCollisionCircleRec(ball.center, ball.radius, (Rectangle){SCREEN_WIDTH - 70, right_paddle_y, 20, 100}))
		{
			ball.speed.x *= -speed_multiplier;
		}

		// Left paddle controls
		if (IsKeyDown(KEY_S))
			left_paddle_y += 5;
		if ((left_paddle_y + 100) >= SCREEN_HEIGHT)
			left_paddle_y = SCREEN_HEIGHT - 100;
		if (IsKeyDown(KEY_W))
			left_paddle_y -= 5;
		if (left_paddle_y <= 0)
			left_paddle_y = 0;

		// Right paddle controls
		if (IsKeyDown(KEY_DOWN))
			right_paddle_y += 5;
		if ((right_paddle_y + 100) >= SCREEN_HEIGHT)
			right_paddle_y = SCREEN_HEIGHT - 100;
		if (IsKeyDown(KEY_UP))
			right_paddle_y -= 5;
		if (right_paddle_y <= 0)
			right_paddle_y = 0;

		BeginDrawing();

		ClearBackground(BLACK);

		DrawText("822", 200, 200, 20, WHITE);

		DrawRectangle(50, left_paddle_y, 20, 100, WHITE);
		DrawRectangle(SCREEN_WIDTH - 70, right_paddle_y, 20, 100, WHITE);
		DrawCircleV(ball.center, ball.radius, WHITE);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}