#include "player.h"
#include "game.h"
#include "globals.h"
#include "map_tiles.h"
#include "raylib.h"
#include "rooms.h"
#include "enemies.h"
#include "stdio.h"

Player player;

void player_Init() {
    player.elevation = 0;
    player.previous_elevation = 0;
    player.y = 0;
    player.x = 0;
    player.previous_x = 0;
    player.previous_y = 0;
    player.target_block_x = 0;
    player.target_block_y = 0;

    player.has_won  = false;
    player.has_lost = false;

    player.has_key  = false;

    player.move_timer = 0.0f;
}

void player_Update() {
    glob_delta_time = GetFrameTime();
    player.move_timer += glob_delta_time;

    if (player.move_timer >= GLOBAL_PLAYER_MOVE_COOLDOWN) {
        // Store the original position
        float original_x = player.x;
        float original_y = player.y;

        // Handle movement in different directions
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
            player_Move_Player_And_Pushable_Block(player.x, player.y - 1, 0, -1);
        }

        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
            player_Move_Player_And_Pushable_Block(player.x - 1, player.y, -1, 0);
        }

        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
            player_Move_Player_And_Pushable_Block(player.x, player.y + 1, 0, 1);
        }

        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
            player_Move_Player_And_Pushable_Block(player.x + 1, player.y, 1, 0);
        }

        // Check if the player has moved to a new position
        if (player.x != original_x || player.y != original_y) {

            //reset the elevation of the previous position if it was a pushable block
            int prev_x = (int)original_x;
            int prev_y = (int)original_y;
            int prev_tile = room_1.layout[prev_x][prev_y][ROOM_GROUND_FLOOR];

            // Only reset elevation if the previous tile was a pushable block
            if(prev_tile == ROOM_TILE_PUSHABLE_BLOCK ||
               prev_tile == ROOM_TILE_WALL){
                room_1.elevation[prev_x][prev_y] = room_1.original_elevation[prev_x][prev_y];
            }

            // //handle current position elevation
            // int current_tile = room_1.layout[(int)player.x][(int)player.y][ROOM_GROUND_FLOOR];
            // if(current_tile == ROOM_TILE_PUSHABLE_BLOCK){
            //     //only raise the block if the player is standing on it
            //     room_1.elevation[(int)player.x][(int)player.y] = room_1.original_elevation[(int)player.x][(int)player.y] + 1;
            //     player.elevation = room_1.elevation[(int)player.x][(int)player.y];
            // }
            // else{
            //     player.elevation = room_1.original_elevation[(int)player.x][(int)player.y];
            // }

            //update the previous position
            player.previous_x = original_x;
            player.previous_y = original_y;

            //reset the movement timer
            player.move_timer = 0.0f;

            // // Only lower the elevation if the player's previous elevation is different
            // if (room_1.elevation[(int)player.previous_x][(int)player.previous_y] == (int)player.elevation) {
            //     lower_Block_Elevation(player.previous_x, player.previous_y, player.elevation - 1);
            // }
            //
            // // Update the previous position to the original position
            // player.previous_x = original_x;
            // player.previous_y = original_y;
            //
            // // Reset the movement timer
            // player.move_timer = 0.0f;
            //
            //
            // for(int i = 0; i < GLOBAL_GRID_WIDTH; i++)
            // {
            //     for(int j = 0; j < GLOBAL_GRID_HEIGHT; j++){
            //
            //         int tileType = room_1.layout[i][j][ROOM_GROUND_FLOOR];
            //
            //
            //         if(player.elevation == ROOM_GROUND_FLOOR && tileType == ROOM_TILE_PUSHABLE_BLOCK){
            //             room_1.layout[i][j][ROOM_GROUND_FLOOR] = ROOM_TILE_PUSHABLE_BLOCK;
            //             room_1.elevation[i][j]                 = ROOM_FIRST_FLOOR;
            //         }
            //
            //         if(player.x == i && player.y == j){
            //             player.elevation--;
            //         }
            //     }
            // }
        }

        // Handle reset key
        if (IsKeyPressed(KEY_R) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP)) {
            game_Reset();
        }
    }
}


// void player_Update() {
//     glob_delta_time = GetFrameTime();
//     player.move_timer += glob_delta_time;
//
//     if (player.move_timer >= GLOBAL_PLAYER_MOVE_COOLDOWN) {
//         // Store the original position
//         float original_x = player.x;
//         float original_y = player.y;
//
//         // Handle movement in different directions
//         if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
//             player_Move_Player_And_Pushable_Block(player.x, player.y - 1, 0, -1);
//         }
//
//         if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
//             player_Move_Player_And_Pushable_Block(player.x - 1, player.y, -1, 0);
//         }
//
//         if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
//             player_Move_Player_And_Pushable_Block(player.x, player.y + 1, 0, 1);
//         }
//
//         if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
//             player_Move_Player_And_Pushable_Block(player.x + 1, player.y, 1, 0);
//         }
//
//         // Check if the player has moved to a new position
//         if (player.x != original_x || player.y != original_y) {
//             // Lower the elevation of the block at the previous position
//             lower_Block_Elevation(player.previous_x, player.previous_y, player.elevation - 1);
//
//             // Update the previous position to the original position
//             player.previous_x = original_x;
//             player.previous_y = original_y;
//
//             // Reset the movement timer
//             player.move_timer = 0.0f;
//         }
//
//         // Handle reset key
//         if (IsKeyPressed(KEY_R) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP)) {
//             game_Reset();
//         }
//     }
// }

void lower_Block_Elevation(float x, float y, float new_elevation) {
    // Ensure the new elevation is not negative
    if (new_elevation < 0) {
        new_elevation = 0;
    }

    // Get the grid position based on x, y (you might need to adjust this depending on how you use x, y)
    int grid_x = (int)x; // Assuming x is an integer position, adjust as needed
    int grid_y = (int)y; // Assuming y is an integer position, adjust as needed

    // Check if the coordinates are valid within the grid
    if (grid_x >= 0 && grid_x < GLOBAL_GRID_WIDTH && grid_y >= 0 && grid_y < GLOBAL_GRID_HEIGHT) {
        // Update the elevation of the block at the given position
        room_1.elevation[grid_x][grid_y] = (int)new_elevation;
    }
}

void player_Draw(int x, int y) {
    // int text_size = 20;

    if (x == player.x && y == player.y) {
        Vector2 player_screen_pos = map_Grid_To_Screen(player.x, player.y, room_1.elevation[x][y]);
        DrawTexture(player.texture, player_screen_pos.x + (float)GLOBAL_SCREEN_WIDTH / 2.0f , (float)player_screen_pos.y , WHITE);
        // DrawText(TextFormat("x:%d, y:%d, e:%d", x, y, room_1.elevation[(int)player.x][(int)player.y]), player_screen_pos.x + (float)GLOBAL_SCREEN_WIDTH / 2.0f, (float)player_screen_pos.y - 64.0f , text_size, WHITE);
        // DrawText(TextFormat("prev_x:%d, prev_y%d",           (int)player.previous_x, (int)player.previous_y), player_screen_pos.x + (float)GLOBAL_SCREEN_WIDTH / 2.0f + 64, (float)player_screen_pos.y, text_size, WHITE);
        // DrawText(TextFormat("elev:%d",               room_1.elevation[(int)player.x][(int)player.y]), player_screen_pos.x + (float)GLOBAL_SCREEN_WIDTH / 2.0f + 32, (float)player_screen_pos.y - 32.0f , text_size, WHITE);
    }
}

void player_Move_Player_And_Pushable_Block(int targetPlayerX, int targetPlayerY, int dirX, int dirY) {

    if (room_1.elevation[targetPlayerX][targetPlayerY] > ROOM_NUMBER_OF_FLOORS) {
        // Print an error message to the console
        fprintf(stderr, "ERROR: ELEVATION OUT OF BOUNDS IN FILE: %s at line %d\n", __FILE__, __LINE__);

        // Optionally, you can also log this information to a file
        FILE *logFile = fopen("error_log.txt", "a");
        if (logFile) {
            fprintf(logFile, "ERROR: ELEVATION OUT OF BOUNDS IN FILE: %s at line %d\n", __FILE__, __LINE__);
            fclose(logFile);
        }
    }

    int targetBlockX = targetPlayerX + dirX;
    int targetBlockY = targetPlayerY + dirY;

    if (map_Can_Move_To_Position(targetPlayerX, targetPlayerY)) {
        int currentTile = room_1.layout[targetPlayerX][targetPlayerY][ROOM_GROUND_FLOOR];
        // int previousTile = room_1.layout[(int)player.x][(int)player.y][ROOM_GROUND_FLOOR];
        int targetOriginalElevation = room_1.original_elevation[targetPlayerX][targetPlayerY];

        // Check if the player is trying to push a block
        if (currentTile == ROOM_TILE_PUSHABLE_BLOCK) {
            // Ensure the block can be moved
            if (player_Can_Move_Pushable_Block(targetBlockX, targetBlockY)) {
                // Move the block
                room_1.layout[targetBlockX][targetBlockY][ROOM_GROUND_FLOOR] = ROOM_TILE_PUSHABLE_BLOCK;
                room_1.layout[targetPlayerX][targetPlayerY][ROOM_GROUND_FLOOR] = ROOM_TILE_FLOOR; // Clear the old block position

                // Reset the elevation of the old block position to its original value
                room_1.elevation[targetPlayerX][targetPlayerY] = targetOriginalElevation;

                // Update the player's position
                player.x = targetPlayerX;
                player.y = targetPlayerY;

                // Maintain the player's elevation at the target position
                // room_1.elevation[(int)player.x][(int)player.y] = targetElevation;
                player.elevation = targetOriginalElevation;
                room_1.elevation[(int)player.x][(int)player.y] = player.elevation;

            } else {
                // Block cannot be moved, allow standing on it
                // previousTile = room_1.layout[(int)player.x][(int)player.y][room_1.elevation[(int)player.x][(int)player.y]];
                player.x = targetPlayerX;
                player.y = targetPlayerY;

                room_1.elevation[(int)player.x][(int)player.y] = targetOriginalElevation + 1;
                player.elevation = room_1.elevation[(int)player.x][(int)player.y];
                // only increment if we're not moving back to a previous push block'
                // if(previousTile != room_1.layout[(int)player.x][(int)player.y][room_1.elevation[targetPlayerX][targetPlayerY]]){
                //     // Set the player's elevation to the block's elevation + 1
                //     if (room_1.elevation[(int)player.x][(int)player.y] != targetElevation + 1)
                //     {
                //         room_1.elevation[(int)player.x][(int)player.y] = targetElevation + 1;
                //     }
                // }
            }
        }else if (currentTile == ROOM_TILE_WALL){
            player.x = targetPlayerX;
            player.y = targetPlayerY;

            room_1.elevation[(int)player.x][(int)player.y] = targetOriginalElevation + 1;
            player.elevation = room_1.elevation[(int)player.x][(int)player.y];
        }
        else {
            // If the player is not standing on the block, reset the elevation of the tile they were on
            //room_1.elevation[(int)player.x][(int)player.y]          = room_1.original_elevation[(int)player.x][(int)player.y];

            // Check if there's an enemy at the target position
            bool enemy_in_target = false;
            for (int i = 0; i < GLOBAL_MAX_ENEMIES; i++) {
                if (enemy_x[i] == targetPlayerX && enemy_y[i] == targetPlayerY) {
                    enemy_in_target = true;
                    break;
                }
            }

            // Only allow movement if there is no enemy
            if (!enemy_in_target) {
                // Update the player's position
                player.x = targetPlayerX;
                player.y = targetPlayerY;

                // Reset elevation to the target tile's elevation
                player.elevation = targetOriginalElevation;
                room_1.elevation[(int)player.x][(int)player.y] = player.elevation;
            }
        }
    }
}

bool player_Can_Move_Pushable_Block(int target_block_x, int target_block_y) {
    if (target_block_x < 0 || target_block_x >= GLOBAL_GRID_WIDTH || target_block_y < 0 || target_block_y >= GLOBAL_GRID_HEIGHT) {
        return false;
    }

    //prevent block from moving if the player will move to it from above
    int player_elevation = room_1.elevation[(int)player.x][(int)player.y];
    if(player_elevation > room_1.elevation[target_block_x][target_block_y])
    {
        return false;
    }

    int targetBlockType = room_1.layout[target_block_x][target_block_y][ROOM_GROUND_FLOOR];
    if (targetBlockType == ROOM_TILE_FLOOR) {
        return true;
    } else if (targetBlockType == ROOM_TILE_WALL || targetBlockType == ROOM_TILE_GEM || targetBlockType == ROOM_TILE_TREE || targetBlockType == ROOM_TILE_DOOR || targetBlockType == ROOM_TILE_ELEVATED || targetBlockType == ROOM_TILE_ELEVATED_2) {
        return false;
    }

    return false;
}
