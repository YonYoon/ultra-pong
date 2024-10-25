#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;

int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Eight Two Two");

	SearchAndSetResourceDir("resources");
	
	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(BLACK);

		DrawText("822", 200,200,20,WHITE);

		DrawRectangle(50,20,20,100,WHITE);
		DrawRectangle(SCREEN_WIDTH - 70,20,20,100,WHITE);
		DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 15.0, WHITE);
		
		EndDrawing();
	}

	CloseWindow();
	return 0;
}