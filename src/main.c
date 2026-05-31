#include "raylib.h"
#include "globals.h"
#include "game.h"

int main() {

    InitWindow(CONSTANT_SCREEN_WIDTH, CONSTANT_SCREEN_HEIGHT, "Isometric Game with Raylib");

    game_Init();
	
    SetTargetFPS(60);

		while (!WindowShouldClose()) {
			game_Update();
			game_Draw();
		}

		game_Unload();

    CloseWindow();

    return 0;
} 
