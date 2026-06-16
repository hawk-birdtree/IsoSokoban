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

void game_Init(void) {

    game_timer    = 200.0f; // Timer starts at 10 seconds
    game_maxTimer = 200.0f;

    game_rectWidth  = 750.0f; 
    game_rectHeight =  10.0f; 
    game_rectX      =  10.0f; 
    game_rectY      =   8.0f;

    HideCursor();
    global_Init_Camera();
    textures_Init();
    room_Load_From_png(&room_1, "../out/room_1_ground_floor.png", "../out/room_1_first_floor.png", "../out/room_1_second_floor.png");
    room_Load_From_png(&room_2, "../out/room_2_ground_floor.png", "../out/room_2_first_floor.png", "../out/room_2_second_floor.png");
    enemy_Init();
    player_Init();
}

void game_Load_Next_Room(Room* next_room, const char* floor_0_path, const char* floor_1_path, const char* floor_2_path)
{
        global_active_room = next_room;
        room_Load_From_png(global_active_room, floor_0_path, floor_1_path, floor_2_path);
        enemy_Init();
        player_Init();
        player.x = global_active_room->start_x;
        player.y = global_active_room->start_y;
        player.has_won = false;
        global_gems = 0;
}

static float rectCurrentWidth;

void game_Update(void) {

    rectCurrentWidth= (game_timer / game_maxTimer) * game_rectWidth;

    if (game_timer > 0.0f) {
        game_timer -= GetFrameTime(); 
        if (game_timer < 0.0f) game_timer = 0.0f;
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

    if (global_active_room->layout[player.x][player.y][ROOM_GROUND_FLOOR] == ROOM_TILE_KEY) {
        room_Pick_Up_Key();
    }

    if (global_active_room->layout[player.x][player.y][ROOM_GROUND_FLOOR] == ROOM_TILE_GEM) {
		room_Pick_Up_Loot();
        global_gems++;
    }

    if (global_active_room->layout[player.x][player.y][ROOM_GROUND_FLOOR] == ROOM_TILE_DOOR && player.has_key && global_gems == CONSTANT_MAX_GEMS) {
        game_Load_Next_Room(&room_2, "../out/room_2_ground_floor.png", "../out/room_2_first_floor.png", "../out/room_2_second_floor.png");
    }

	//show message if player has no key
    if (global_active_room->layout[player.x][player.y][ROOM_GROUND_FLOOR] == ROOM_TILE_DOOR) {
        if (room_Can_Use_Door()) {
            global_active_room->layout[player.x][player.y][ROOM_FIRST_FLOOR] = 0;
            global_active_room->show_door_message = false;
        }
    }
}

void game_Draw(void) {

    BeginDrawing();

    BeginMode2D(global_camera);

    int saturation = 50;
    Color tint = {saturation,saturation,saturation,255};

    ClearBackground(tint);

    for (int y = 0; y < CONSTANT_GRID_HEIGHT; y++) {
        for (int x = 0; x < CONSTANT_GRID_WIDTH; x++) {
            map_Draw(x, y);
            enemy_Draw(x, y);
            if (x == player.x && y == player.y) {
                player_Draw(player.x, player.y);
            }
        }
    }
    
    EndMode2D();

    DrawText(TextFormat(": %d", global_gems), 60, 35, 20, WHITE);
    DrawTexture(texture_gem, 5, 20, WHITE);
    
    for(int i = 0; i < player.max_hp; i++)
    {
        Color hpColor = (i < player.hp) ? RED : DARKGRAY;
        DrawRectangle(10 + (i * 20), 70, 15, 15, hpColor);      
    }

    // Render the picked up key at top corner
    Rectangle keyRect = {20, 20, texture_key.width/1.5f, texture_key.height/1.5f};

    if (player.has_key)
    {
        DrawRectangle(keyRect.x+10, keyRect.y+10, keyRect.width, keyRect.height, BLACK);
        DrawTexture(texture_key, keyRect.x, keyRect.y, WHITE);
    }

    // Draw the "You need a key for this door" message
    if ((player.x == global_active_room->door_x && player.y == global_active_room->door_y) && (player.has_key == false || global_gems < CONSTANT_MAX_GEMS)) {
        int textWidth = MeasureText(TextFormat("I need a key for this door and %d gems", CONSTANT_MAX_GEMS), 20);
        int textHeight = 40;
        Rectangle rec = (Rectangle){ global_active_room->door_x, global_active_room->door_y, textWidth, (float)textHeight };
        float roundness = 0.5;
        int segments = 4;
        DrawRectangleRounded(rec, roundness, segments, BLACK);
        DrawText(TextFormat("I need a key for this door and %d gems", CONSTANT_MAX_GEMS), rec.x, rec.y + 10, 20, RED);
    }

    // Draw the win message if the player has won
    if (player.has_won) {
        DrawRectangle(0, 0, CONSTANT_SCREEN_WIDTH, CONSTANT_SCREEN_HEIGHT, BLACK);
        // Calculate the width of the text to center it on the screen
        int textWidth = MeasureText("Congratulations! You win!", 30);
        int textX = (CONSTANT_SCREEN_WIDTH - textWidth) / 2;
        int textY = CONSTANT_SCREEN_HEIGHT / 2;
        DrawText("Congratulations! You win!", textX, textY, 30, RED);
        textWidth = MeasureText("Press R to start over", 25);
        DrawText("Press R to start over", textX + 10, textY + 50, 25, GRAY);
    }
    
    // Draw the timer rectangle
    DrawRectangle(game_rectX, game_rectY, rectCurrentWidth, game_rectHeight, RED);

    // Draw the win message if the player has lost
    if ((game_timer <= 0 && player.has_won == false) || player.has_lost == true) {
        DrawRectangle(0, 0, CONSTANT_SCREEN_WIDTH, CONSTANT_SCREEN_HEIGHT, BLACK);
        // Calculate the width of the text to center it on the screen
        int textX, textY, textWidth;
        
        if(player.has_lost)
        {
            textWidth = MeasureText("You died!", 40);
            textX = (CONSTANT_SCREEN_WIDTH - textWidth) / 2;
            textY = CONSTANT_SCREEN_HEIGHT / 3;
            DrawText("You died!", textX, textY, 40, RED);
        }
        else
        {
            textWidth = MeasureText("You ran out of time!", 40);
            textX = (CONSTANT_SCREEN_WIDTH - textWidth) / 2;
            textY = CONSTANT_SCREEN_HEIGHT / 3;
            DrawText("You ran out of time!", textX, textY, 40, RED);
        }
        
        textWidth = MeasureText("Press R to start over", 25);
        DrawText("Press R to start over", textX+20, textY + 50, 25, GRAY);
    }

    EndDrawing();
}

void game_Reset(void) {
    texture_Unload();
    game_Init();
}

void game_Unload(void) {
    texture_Unload();
}
