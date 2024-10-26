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

void move_paddle_up(Rectangle *paddle);
void move_paddle_down(Rectangle *paddle);

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ultra Pong");

	InitAudioDevice();

	SetTargetFPS(60);

	SearchAndSetResourceDir("resources");

	Sound fxCollision = LoadSound("collision.wav");
	Sound fxUltra = LoadSound("ultra.wav");
	Sound fxExplosion = LoadSound("explosion.wav");

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

	bool is_right_ultra = false;
	bool is_left_ultra = false;

	while (!WindowShouldClose())
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
			ball.speed.x = 3;
			ball.speed.y = 3;
			PlaySound(fxExplosion);
		}
		if (ball.center.x + ball.radius <= 0)
		{
			right_player_score += 1;
			ball.center.x = SCREEN_WIDTH / 2;
			ball.center.y = SCREEN_HEIGHT / 2;
			ball.speed.x = -3;
			ball.speed.y = 3;
			PlaySound(fxExplosion);
		}

		// Collision with left and right paddle
		if (CheckCollisionCircleRec(ball.center, ball.radius, paddle_left))
		{
			ball.speed.x *= -speed_multiplier;
			if (is_left_ultra)
			{
				PlaySound(fxUltra);
			}
			else
			{
				PlaySound(fxCollision);
			}
			is_left_ultra = true;
			is_right_ultra = false;
		}
		if (CheckCollisionCircleRec(ball.center, ball.radius, paddle_right))
		{
			ball.speed.x *= -speed_multiplier;
			if (is_right_ultra)
			{
				PlaySound(fxUltra);
			}
			else
			{
				PlaySound(fxCollision);
			}
			is_left_ultra = false;
			is_right_ultra = true;
		}

		// Left paddle controls
		if (IsKeyDown(KEY_S))
			move_paddle_down(&paddle_left);
		if (IsKeyDown(KEY_W))
			move_paddle_up(&paddle_left);

		// Right paddle controls
		if (IsKeyDown(KEY_DOWN))
			move_paddle_down(&paddle_right);
		if (IsKeyDown(KEY_UP))
			move_paddle_up(&paddle_right);

		BeginDrawing();

		ClearBackground(WHITE);

		DrawText(TextFormat("%i", left_player_score), 200, 100, 20, BLACK);
		DrawText(TextFormat("%i", right_player_score), SCREEN_WIDTH - 200, 100, 20, BLACK);

		DrawRectangleRec(paddle_left, BLACK);
		DrawRectangleRec(paddle_right, BLACK);
		DrawCircleV(ball.center, ball.radius, BLACK);

		EndDrawing();
	}

	UnloadSound(fxCollision);
	UnloadSound(fxExplosion);
	UnloadSound(fxUltra);

	CloseAudioDevice();

	CloseWindow();
	return 0;
}

void move_paddle_up(Rectangle *paddle)
{
	paddle->y -= 5;
	if (paddle->y <= 0)
		paddle->y = 0;
}

void move_paddle_down(Rectangle *paddle)
{
	paddle->y += 5;
	if ((paddle->y + 100) >= SCREEN_HEIGHT)
		paddle->y = SCREEN_HEIGHT - 100;
}