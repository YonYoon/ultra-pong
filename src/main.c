#include <stdio.h>
#include "raylib.h"
#include "config.h"
#include "ball.h"
#include "paddle.h"

#include "resource_dir.h" // utility header for SearchAndSetResourceDir

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;

const float BALL_SPEED_X = 4.0;
const float SPEED_MULTIPLIER = 1.1;

const int HITS_TO_DASH = 3;

static const int TITLE_FONT_SIZE = 100;
static const int SCORE_FONT_SIZE = 50;
static const int DASH_FONT_SIZE = 30;

typedef enum GameStage
{
	MENU,
	SINGLE_PLAYER_MODE,
	TWO_PLAYER_MODE,
	GAME_OVER,
} GameStage;

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ultra Pong");

	InitAudioDevice();

	SetTargetFPS(60);

	SearchAndSetResourceDir("resources");

	GameStage gameStage = MENU;

	Sound fxCollision = LoadSound("collision.wav");
	Sound fxUltra = LoadSound("ultra.wav");
	Sound fxExplosion = LoadSound("explosion.wav");

	int left_player_score = 0;
	int right_player_score = 0;

	// Ball setup
	Ball ball = ball_setup();

	// Paddles setup
	Paddle paddle_left = paddle_setup(50);
	Paddle paddle_right = paddle_setup(SCREEN_WIDTH - 70);

	// Calculate text dimensions
	int title_text_width = MeasureText("ULTRA PONG", TITLE_FONT_SIZE);
	int restart_text_width = MeasureText("Press SPACE to restart", 50);
	int text_width = MeasureText("S: 1 player", 50);
	int dash_text_width = MeasureText("DASH", DASH_FONT_SIZE);

	while (!WindowShouldClose())
	{
		switch (gameStage)
		{
		case MENU:
		{
			if (IsKeyPressed(KEY_S))
			{
				// reset the game
				right_player_score = 0;
				left_player_score = 0;

				paddle_left.rect.x = 50;
				paddle_left.rect.y = (SCREEN_HEIGHT / 2) - (paddle_left.rect.height / 2);

				paddle_right.rect.x = SCREEN_WIDTH - 70;
				paddle_right.rect.y = (SCREEN_HEIGHT / 2) - (paddle_right.rect.height / 2);

				paddle_left.dash_meter = 0;
				paddle_right.dash_meter = 0;

				gameStage = SINGLE_PLAYER_MODE;
			}
			if (IsKeyPressed(KEY_T))
			{
				// reset the game
				right_player_score = 0;
				left_player_score = 0;

				paddle_left.rect.x = 50;
				paddle_left.rect.y = (SCREEN_HEIGHT / 2) - (paddle_left.rect.height / 2);

				paddle_right.rect.x = SCREEN_WIDTH - 70;
				paddle_right.rect.y = (SCREEN_HEIGHT / 2) - (paddle_right.rect.height / 2);

				paddle_left.dash_meter = 0;
				paddle_right.dash_meter = 0;

				gameStage = TWO_PLAYER_MODE;
			}
			break;
		}
		case GAME_OVER:
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

				paddle_left.dash_meter = 0;
				paddle_right.dash_meter = 0;

				gameStage = TWO_PLAYER_MODE;
			}
			break;
		}

		case SINGLE_PLAYER_MODE:
		{
			update_ball(&ball);
			check_collision_ball_walls(&ball, fxCollision);
			if (is_left_player_goal(&ball, fxExplosion))
			{
				left_player_score++;
				if (left_player_score == 19)
					gameStage = GAME_OVER;
			}
			if (is_right_player_goal(&ball, fxExplosion))
			{
				right_player_score++;
				if (right_player_score == 19)
					gameStage = GAME_OVER;
			}

			// Collision with left and right paddle
			check_collision_ball_paddle(&ball, &paddle_left, fxUltra, fxCollision);
			check_collision_ball_paddle(&ball, &paddle_right, fxUltra, fxCollision);

			if (paddle_left.acceleration > 1.0)
			{
				paddle_left.acceleration -= 0.1;
			}
			if (paddle_right.acceleration > 1.0)
			{
				paddle_right.acceleration -= 0.1;
			}

			// Left paddle controls
			if (IsKeyDown(KEY_W))
				move_paddle_up(&paddle_left);
			if (IsKeyDown(KEY_S))
				move_paddle_down(&paddle_left);

			if (IsKeyDown(KEY_X) && IsKeyDown(KEY_W) || IsKeyDown(KEY_X) && IsKeyDown(KEY_S))
			{
				if (paddle_left.dash_meter >= HITS_TO_DASH)
				{
					dash_paddle(&paddle_left);
					paddle_left.dash_meter = 0;
				}
			}

			// Right paddle AI
			if (ball.speed.x > 0)
			{
				float distance_ball_paddle = paddle_right.rect.x - (ball.center.x + ball.radius);
				float ball_travel_time = distance_ball_paddle / ball.speed.x;
				float ball_Dy = ball.speed.y * ball_travel_time;
				float ball_final_y = ball_Dy + ball.center.y;
				float paddle_center = paddle_right.rect.y + (paddle_right.rect.height / 2);
				if (ball_final_y > paddle_center)
				{
					move_paddle_down(&paddle_right);
				}
				else if (ball_final_y < paddle_center)
				{
					move_paddle_up(&paddle_right);
				}
			}
			break;
		}
		case TWO_PLAYER_MODE:
		{
			update_ball(&ball);
			check_collision_ball_walls(&ball, fxCollision);
			if (is_left_player_goal(&ball, fxExplosion))
			{
				left_player_score++;
				if (left_player_score == 19)
					gameStage = GAME_OVER;
			}
			if (is_right_player_goal(&ball, fxExplosion))
			{
				right_player_score++;
				if (right_player_score == 19)
					gameStage = GAME_OVER;
			}

			// Collision with left and right paddle
			check_collision_ball_paddle(&ball, &paddle_left, fxUltra, fxCollision);
			check_collision_ball_paddle(&ball, &paddle_right, fxUltra, fxCollision);

			if (paddle_left.acceleration > 1.0)
			{
				paddle_left.acceleration -= 0.1;
			}
			if (paddle_right.acceleration > 1.0)
			{
				paddle_right.acceleration -= 0.1;
			}

			// Left paddle controls
			if (IsKeyDown(KEY_W))
				move_paddle_up(&paddle_left);
			if (IsKeyDown(KEY_S))
				move_paddle_down(&paddle_left);

			if (paddle_left.dash_meter >= HITS_TO_DASH)
			{
				if (IsKeyDown(KEY_X) && IsKeyDown(KEY_W) || IsKeyDown(KEY_X) && IsKeyDown(KEY_S))
				{
					dash_paddle(&paddle_left);
					paddle_left.dash_meter = 0;
				}
			}

			// Right paddle controls
			if (IsKeyDown(KEY_I))
				move_paddle_up(&paddle_right);
			if (IsKeyDown(KEY_J))
				move_paddle_down(&paddle_right);

			if (paddle_right.dash_meter >= HITS_TO_DASH)
			{
				if (IsKeyDown(KEY_M) && IsKeyDown(KEY_I) || IsKeyDown(KEY_M) && IsKeyDown(KEY_J))
				{
					dash_paddle(&paddle_right);
					paddle_right.dash_meter = 0;
				}
			}
			break;
		}
		}

		BeginDrawing();

		ClearBackground(WHITE);

		switch (gameStage)
		{
		case MENU:
		{
			DrawText("ULTRA PONG", (SCREEN_WIDTH / 2) - (title_text_width / 2), SCREEN_HEIGHT / 2 - 100, TITLE_FONT_SIZE, RED);
			DrawText("S: 1 player", (SCREEN_WIDTH / 2) - (text_width / 2), SCREEN_HEIGHT / 2 + 50, 50, BLACK);
			DrawText("T: 2 player", (SCREEN_WIDTH / 2) - (text_width / 2), SCREEN_HEIGHT / 2 + 140, 50, BLACK);
		}
		break;

		case GAME_OVER:
		{
			DrawText(TextFormat("%i", left_player_score), 200, 100, SCORE_FONT_SIZE, BLACK);
			DrawText(TextFormat("%i", right_player_score), SCREEN_WIDTH - 200, 100, SCORE_FONT_SIZE, BLACK);

			DrawText("GAME OVER", (SCREEN_WIDTH / 2) - (title_text_width / 2), SCREEN_HEIGHT / 2 - 100, TITLE_FONT_SIZE, RED);
			DrawText("Press SPACE to restart", (SCREEN_WIDTH / 2) - (restart_text_width / 2), SCREEN_HEIGHT / 2, 50, BLACK);
		}
		break;

		case SINGLE_PLAYER_MODE:
		case TWO_PLAYER_MODE:
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