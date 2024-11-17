#include <stdio.h>
#include "raylib.h"
#include "config.h"
#include "ball.h"
#include "paddle.h"

#include "resource_dir.h" // utility header for SearchAndSetResourceDir

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;

const float BALL_SPEED_X = 4.0;

const int HITS_TO_DASH = 3;

static const int TITLE_FONT_SIZE = 100;
static const int SCORE_FONT_SIZE = 50;
static const int DASH_FONT_SIZE = 30;

typedef enum GameScreen
{
	START,
	GAMEPLAY,
	ENDING,
} GameScreen;

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ultra Pong");

	InitAudioDevice();

	SetTargetFPS(60);

	SearchAndSetResourceDir("resources");

	GameScreen currentScreen = START;

	Sound fxCollision = LoadSound("collision.wav");
	Sound fxUltra = LoadSound("ultra.wav");
	Sound fxExplosion = LoadSound("explosion.wav");

	int left_player_score = 0;
	int right_player_score = 0;

	// Ball setup
	Ball ball = ball_setup();

	const float speed_multiplier = 1.1;

	// Paddles setup
	Paddle paddle_left = paddle_setup(50);
	Paddle paddle_right = paddle_setup(SCREEN_WIDTH - 70);

	bool is_right_ultra = false;
	bool is_left_ultra = false;

	// Calculate text dimensions
	int title_text_width = MeasureText("ULTRA PONG", TITLE_FONT_SIZE);
	int text_width = MeasureText("Press SPACE to start", 50);
	int dash_text_width = MeasureText("DASH", DASH_FONT_SIZE);

	while (!WindowShouldClose())
	{
		switch (currentScreen)
		{
		case START:
		case ENDING:
		{
			if (IsKeyPressed(KEY_SPACE))
			{
				// reset the game
				right_player_score = 0;
				left_player_score = 0;

				paddle_left.rect.x = 50;
				paddle_left.rect.y = (SCREEN_HEIGHT / 2) - (paddle_left.rect.height / 2);

				paddle_right.rect.x = SCREEN_WIDTH - 70;
				paddle_right.rect.y = (SCREEN_HEIGHT / 2) - (paddle_right.rect.height / 2);

				paddle_left.can_dash = false;
				paddle_right.can_dash = false;

				paddle_left.dash_meter = 0;
				paddle_right.dash_meter = 0;

				currentScreen = GAMEPLAY;
			}
		}
		break;

		case GAMEPLAY:
		{
			ball.center.x += ball.speed.x;
			ball.center.y += ball.speed.y;

			// Collision with screen bounds
			if (ball.center.y - ball.radius <= 0)
			{
				ball.speed.y *= -speed_multiplier;
				PlaySound(fxCollision);
			}
			if (ball.center.y + ball.radius >= SCREEN_HEIGHT)
			{
				ball.speed.y *= -speed_multiplier;
				PlaySound(fxCollision);
			}
			if (ball.center.x - ball.radius >= SCREEN_WIDTH)
			{
				left_player_score += 1;
				ball.center.x = SCREEN_WIDTH / 2;
				ball.center.y = SCREEN_HEIGHT / 2;
				ball.speed.x = BALL_SPEED_X;
				ball.speed.y = random_speed_y();
				PlaySound(fxExplosion);
			}
			if (ball.center.x + ball.radius <= 0)
			{
				right_player_score += 1;
				ball.center.x = SCREEN_WIDTH / 2;
				ball.center.y = SCREEN_HEIGHT / 2;
				ball.speed.x = -BALL_SPEED_X;
				ball.speed.y = random_speed_y();
				PlaySound(fxExplosion);
			}

			if (right_player_score == 19 || left_player_score == 19)
			{
				currentScreen = ENDING;
			}

			// Collision with left and right paddle
			if (CheckCollisionCircleRec(ball.center, ball.radius, paddle_left.rect))
			{
				ball.speed.x *= -speed_multiplier;
				if (paddle_left.speed > 0 && ball.speed.y < 0)
				{
					ball.speed.y *= -1;
				}
				else if (paddle_left.speed < 0 && ball.speed.y > 0)
				{
					ball.speed.y *= -1;
				}
				if (is_left_ultra)
					PlaySound(fxUltra);
				else
					PlaySound(fxCollision);
				if (!is_left_ultra)
				{
					paddle_left.dash_meter += 1;
				}
				is_left_ultra = true;
				is_right_ultra = false;
			}
			if (CheckCollisionCircleRec(ball.center, ball.radius, paddle_right.rect))
			{
				ball.speed.x *= -speed_multiplier;
				if (paddle_right.speed > 0 && ball.speed.y < 0)
				{
					ball.speed.y *= -1;
				}
				else if (paddle_right.speed < 0 && ball.speed.y > 0)
				{
					ball.speed.y *= -1;
				}
				if (is_right_ultra)
					PlaySound(fxUltra);
				else
					PlaySound(fxCollision);
				if (!is_right_ultra)
				{
					paddle_right.dash_meter += 1;
				}
				is_left_ultra = false;
				is_right_ultra = true;
			}

			// Limit ball's speed
			if (ball.speed.x > 45.0)
			{
				ball.speed.x = 45.0;
			}
			else if (ball.speed.x < -45.0)
			{
				ball.speed.x = -45.0;
			}

			if (paddle_left.acceleration > 1.0)
			{
				paddle_left.can_dash = false;
				paddle_left.acceleration -= 0.1;
			}
			if (paddle_right.acceleration > 1.0)
			{
				paddle_right.can_dash = false;
				paddle_right.acceleration -= 0.1;
			}

			// Left paddle controls
			if (IsKeyDown(KEY_S))
				move_paddle_down(&paddle_left);
			if (IsKeyDown(KEY_W))
				move_paddle_up(&paddle_left);

			if (paddle_left.can_dash || paddle_left.dash_meter >= HITS_TO_DASH)
			{
				if (IsKeyDown(KEY_X))
				{
					dash_paddle(&paddle_left);
					paddle_left.dash_meter = 0;
				}
			}

			// Right paddle controls
			if (IsKeyDown(KEY_J))
				move_paddle_down(&paddle_right);
			if (IsKeyDown(KEY_I))
				move_paddle_up(&paddle_right);

			if (paddle_right.can_dash || paddle_right.dash_meter >= HITS_TO_DASH)
			{
				if (IsKeyDown(KEY_M))
				{
					dash_paddle(&paddle_right);
					paddle_right.dash_meter = 0;
				}
			}
		}
		break;
		}

		BeginDrawing();

		ClearBackground(WHITE);

		switch (currentScreen)
		{
		case START:
		{
			DrawText("ULTRA PONG", (SCREEN_WIDTH / 2) - (title_text_width / 2), SCREEN_HEIGHT / 2 - 100, TITLE_FONT_SIZE, RED);
			DrawText("Press SPACE to start", (SCREEN_WIDTH / 2) - (text_width / 2), SCREEN_HEIGHT / 2, 50, BLACK);
		}
		break;

		case ENDING:
		{
			DrawText(TextFormat("%i", left_player_score), 200, 100, SCORE_FONT_SIZE, BLACK);
			DrawText(TextFormat("%i", right_player_score), SCREEN_WIDTH - 200, 100, SCORE_FONT_SIZE, BLACK);

			DrawText("GAME OVER", (SCREEN_WIDTH / 2) - (title_text_width / 2), SCREEN_HEIGHT / 2 - 100, TITLE_FONT_SIZE, RED);
			DrawText("Press SPACE to restart", (SCREEN_WIDTH / 2) - (text_width / 2), SCREEN_HEIGHT / 2, 50, BLACK);
		}
		break;

		case GAMEPLAY:
		{
			DrawText(TextFormat("%i", left_player_score), 200, 100, SCORE_FONT_SIZE, BLACK);
			DrawText(TextFormat("%i", right_player_score), SCREEN_WIDTH - 200, 100, SCORE_FONT_SIZE, BLACK);

			if (paddle_left.dash_meter >= HITS_TO_DASH)
			{
				DrawText("DASH", 100, 50, DASH_FONT_SIZE, BLUE);
			}
			if (paddle_right.dash_meter >= HITS_TO_DASH)
			{
				DrawText("DASH", SCREEN_WIDTH - 100 - dash_text_width, 50, DASH_FONT_SIZE, BLUE);
			}

			DrawRectangleRec(paddle_left.rect, BLACK);
			DrawRectangleRec(paddle_right.rect, BLACK);
			DrawCircleV(ball.center, ball.radius, BLACK);
		}
		break;
		}

		EndDrawing();
	}

	UnloadSound(fxCollision);
	UnloadSound(fxExplosion);
	UnloadSound(fxUltra);

	CloseAudioDevice();

	CloseWindow();
	return 0;
}