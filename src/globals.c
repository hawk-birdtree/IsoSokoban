#include "raylib.h"
#include "map_tiles.h"
#include "rooms.h"
#include "player.h"
#include "globals.h"
#include "raymath.h"
#include "textures.h"

Camera2D glob_camera;
int      glob_gems = 0;
float    glob_delta_time = 0.0f;
int      glob_current_room = 1;
Room    *glob_active_room = &room_1;

void global_Init_Camera() {
	glob_camera.target   = (Vector2){0.0f, 0.0f};
	glob_camera.offset   = (Vector2){0.0f, (float)GLOBAL_SCREEN_HEIGHT/3.0f};
	glob_camera.rotation = 0.0f;
	glob_camera.zoom     = 0.9f;
}

void global_Camera_Update() {
    Vector2 playerScreenPos = map_Grid_To_Screen(player.x, player.y, room_1.elevation[(int)player.x][(int)player.y]);

    // Define a lerp speed (you can adjust this value)
    float lerpSpeed = 0.02f;  // 3;

    // Use Lerp to smoothly move the camera towards the target position
    glob_camera.target.x = Lerp(glob_camera.target.x, playerScreenPos.x, lerpSpeed);
    glob_camera.target.y = Lerp(glob_camera.target.y, playerScreenPos.y, lerpSpeed);
}

void global_Draw_Game_Messages(void) {
    // Render the picked up key at top corner
    Rectangle keyRect = {10, 70, texture_key.width/1.5f, texture_key.height/1.5f};

    if (player.has_key)
    {
        DrawRectangle(keyRect.x+10, keyRect.y+10, keyRect.width, keyRect.height, BLACK);
        DrawTexture(texture_key, keyRect.x, keyRect.y, WHITE);
    }

    // Draw the "You need a key for this door" message
    if ((player.x == room_1.door_x && player.y == room_1.door_y) && (player.has_key == false || glob_gems < GLOBAL_MAX_GEMS)) {
        int textWidth = MeasureText(TextFormat("I need a key for this door and %d gems", GLOBAL_MAX_GEMS), 20);
        int textHeight = 40;
        int textX = (GLOBAL_SCREEN_WIDTH - textWidth) / 2;
        int textY =  14; // SCREEN_HEIGHT / 2 - textHeight;
        Rectangle rec = (Rectangle){ textX, textY - 10, textWidth, (float)textHeight };
        float roundness = 0.5;
        int segments = 4;
        DrawRectangleRounded(rec, roundness, segments, BLACK);
        DrawText(TextFormat("I need a key for this door and %d gems", GLOBAL_MAX_GEMS), rec.x, rec.y + 10, 20, RED);
    }

    // Draw the win message if the player has won
    if (player.has_won) {
        DrawRectangle(0, 0, GLOBAL_SCREEN_WIDTH, GLOBAL_SCREEN_HEIGHT, BLACK);
        // Calculate the width of the text to center it on the screen
        int textWidth = MeasureText("Congratulations! You win!", 30);
        int textX = (GLOBAL_SCREEN_WIDTH - textWidth) / 2;
        int textY = GLOBAL_SCREEN_HEIGHT / 2;
        DrawText("Congratulations! You win!", textX, textY, 30, RED);
        textWidth = MeasureText("Press R to start over", 25);
        DrawText("Press R to start over", textX + 10, textY + 50, 25, GRAY);
    }
}

