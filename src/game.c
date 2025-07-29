#include "game.h"
#include "globals.h"
#include "textures.h"
#include "rooms.h"
#include "enemies.h"
#include "player.h"
#include "map_tiles.h"

float game_timer;
float game_maxTimer;
float game_rectWidth;
float game_rectHeight;
float game_rectX;
float game_rectY;


void game_Init() {

    game_timer    = 200.0f; // Timer starts at 10 seconds
    game_maxTimer = 200.0f; // Maximum timer value

    game_rectWidth  = 750.0f; // Starting width of the rectangle
    game_rectHeight =  10.0f; // Height of the rectangle
    game_rectX      =  10.0f; // X position of the rectangle
    game_rectY      =   8.0f; // Y position of the rectangle

    HideCursor();
    global_Init_Camera();
    textures_Init();
    room_Init_Ground_Floor(&room_1);
    room_Init_First_Floor(&room_1, 0, 0);
    enemy_Init();
    player_Init();
}

static float rectCurrentWidth; // = (game_timer / game_maxTimer) * game_rectWidth;

void game_Update() {

    rectCurrentWidth= (game_timer / game_maxTimer) * game_rectWidth;

    if (game_timer > 0.0f) {
        game_timer -= GetFrameTime(); // Decrease timer by the frame time
        if (game_timer < 0.0f) game_timer = 0.0f; // Clamp the timer to zero
    }

    if(game_timer <= 0)
    {
        player.has_lost = true;
        player.move_timer = 0.0f;
        enemy_speed = 0;
    }


	global_Camera_Update();
	player_Update();
    enemy_Update_Position();

    // Check if the player is on the key tile and pick it up
    if (room_1.layout[(int)player.x][(int)player.y][ROOM_GROUND_FLOOR] == ROOM_TILE_KEY) {
        room_Pick_Up_Key();
    }

    //check for player collision with loot block
    if (room_1.layout[(int)player.x][(int)player.y][ROOM_GROUND_FLOOR] == ROOM_TILE_GEM) {
		room_Pick_Up_Loot();
        glob_gems++;
    }

    // Check for key and is at door
    if (room_1.layout[(int)player.x][(int)player.y][ROOM_GROUND_FLOOR] == ROOM_TILE_DOOR && player.has_key && glob_gems == GLOBAL_MAX_GEMS) {
        player.has_won = true;
    }

	//show message if player has no key
    if (room_1.layout[(int)player.x][(int)player.y][ROOM_GROUND_FLOOR] == ROOM_TILE_DOOR) {
        if (room_Can_Use_Door()) {
            room_1.layout[(int)player.x][(int)player.y][ROOM_FIRST_FLOOR] = 0;
            room_1.show_door_message = false;
        }
    }
}

void game_Draw() {

    BeginDrawing();

    BeginMode2D(glob_camera);

    int saturation = 50;
    Color tint = {saturation,saturation,saturation,255};

    ClearBackground(tint);

    for (int y = 0; y < GLOBAL_GRID_HEIGHT; y++) {
        for (int x = 0; x < GLOBAL_GRID_WIDTH; x++) {
            map_Draw(x,y);
            enemy_Draw(x,y);
            player_Draw(x,y);
        }
    }
    EndMode2D();

    DrawText(TextFormat(": %d", glob_gems), 60, 35, 20, WHITE);
    DrawTexture(texture_gem, 5, 10, WHITE);

    // Draw the timer rectangle
    // rectCurrentWidth = (game_timer / game_maxTimer) * game_rectWidth;
    DrawRectangle(game_rectX, game_rectY, rectCurrentWidth, game_rectHeight, RED);

    global_Draw_Game_Messages();

    // Draw the win message if the player has won
    if (game_timer <= 0 && player.has_won == false) {
        DrawRectangle(0, 0, GLOBAL_SCREEN_WIDTH, GLOBAL_SCREEN_HEIGHT, BLACK);
        // Calculate the width of the text to center it on the screen
        int textWidth = MeasureText("You ran out of time!", 40);
        int textX = (GLOBAL_SCREEN_WIDTH - textWidth) / 2;
        int textY = GLOBAL_SCREEN_HEIGHT / 3;
        DrawText("You ran out of time!", textX, textY, 40, RED);
        textWidth = MeasureText("Press R to start over", 25);
        DrawText("Press R to start over", textX+20, textY + 50, 25, GRAY);

        if (IsKeyPressed(KEY_R) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP)) {
            game_Reset();
        }
    }

    EndDrawing();
}

void game_Reset() {
    game_Init();
}

void game_Unload() {
    texture_Unload();
}
