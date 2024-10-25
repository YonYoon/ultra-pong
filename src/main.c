#include "raylib.h"

#include "resource_dir.h" // utility header for SearchAndSetResourceDir

static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 800;

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

	float ball_x = SCREEN_WIDTH / 2;
	float ball_y = SCREEN_HEIGHT / 2;
	float ball_radius = 15.0;

	float ball_speed_horizontal = 3.0;
	float ball_speed_vertical = 3.0;

	float speed_multiplier = 1.1;

	while (!WindowShouldClose())
	{
		ball_x += ball_speed_horizontal;
		ball_y += ball_speed_vertical;

		// Collision with screen bounds
		if (ball_y - ball_radius <= 0)
		{
			ball_speed_vertical *= -speed_multiplier;
		}
		if (ball_y + ball_radius >= SCREEN_HEIGHT)
		{
			ball_speed_vertical *= -speed_multiplier;
		}
		if (ball_x - ball_radius >= SCREEN_WIDTH)
		{
			left_player_score += 1;
			ball_x = SCREEN_WIDTH / 2;
			ball_y = SCREEN_HEIGHT / 2;
			ball_speed_horizontal = 3;
			ball_speed_vertical = 3;
		}
		if (ball_x + ball_radius <= 0)
		{
			right_player_score += 1;
			ball_x = SCREEN_WIDTH / 2;
			ball_y = SCREEN_HEIGHT / 2;
			ball_speed_horizontal = -3;
			ball_speed_vertical = 3;
		}

		// Collision with left paddle
		if (CheckCollisionCircleRec((Vector2){ball_x, ball_y}, ball_radius, (Rectangle){50, left_paddle_y, 20, 100}))
		{
			ball_speed_horizontal *= -speed_multiplier;
		}

		// Collision with right paddle
		if (CheckCollisionCircleRec((Vector2){ball_x, ball_y}, ball_radius, (Rectangle){SCREEN_WIDTH - 70, right_paddle_y, 20, 100}))
		{
			ball_speed_horizontal *= -speed_multiplier;
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
		DrawCircle(ball_x, ball_y, ball_radius, WHITE);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}