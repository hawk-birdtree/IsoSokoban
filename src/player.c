#include "player.h"
#include "game.h"
#include "globals.h"
#include "map_tiles.h"
#include "raylib.h"
#include "rooms.h"
#include "enemies.h"

Player player;

void player_Init(void) {
    player.elevation = 0;
    player.previous_elevation = 0;
    player.max_hp = 3;
    player.hp = player.max_hp;
    player.x = global_active_room->start_x;
    player.y = global_active_room->start_y;
    player.previous_x = 0;
    player.previous_y = 0;
    player.target_block_x = 0;
    player.target_block_y = 0;
    player.i_frame_counter = 0;
    player.direction = 1;
    player.has_won  = false;
    player.has_lost = false;
    player.is_invincible = false;

    player.has_key  = false;
    global_gems = 0;

    player.move_timer = 0.0f;
}

void player_Update(void) {
    global_delta_time = GetFrameTime();
    player.move_timer += global_delta_time;
    
    if(player.is_invincible)
    {
        player.i_frame_counter--;
        if(player.i_frame_counter <= 0)
        {
            player.is_invincible = false;
        }
    }
    else
    {
        for(int i = 0; i < CONSTANT_MAX_ENEMIES; i++)
        {
            if(enemy_x[i] == player.x && enemy_y[i] == player.y)
            {
                player.hp--;
                player.is_invincible = true;
                player.i_frame_counter = 60;
                player.x = global_active_room->start_x;
                player.y = global_active_room->start_y;
                enemy_Reset_Positions();
                if(player.hp <= 0)
                {
                    player.has_lost = true;
                }
                break;
            }
        }
    }

    if (player.move_timer >= CONSTANT_PLAYER_MOVE_COOLDOWN) {
        // Store the original position
        float original_x = player.x;
        float original_y = player.y;

        // Handle movement in different directions
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
            player_Move_Player_And_Pushable_Block(player.x, player.y - 1, 0, -1);
        }

        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
            player.direction = 1;
            player_Move_Player_And_Pushable_Block(player.x - 1, player.y, -1, 0);
        }

        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
            player_Move_Player_And_Pushable_Block(player.x, player.y + 1, 0, 1);
        }

        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
            player.direction = -1;
            player_Move_Player_And_Pushable_Block(player.x + 1, player.y, 1, 0);
        }

        // Check if the player has moved to a new position
        if (player.x != original_x || player.y != original_y) {

            //reset the elevation of the previous position if it was a pushable block
            int prev_x = original_x;
            int prev_y = original_y;
            int prev_tile = global_active_room->layout[prev_x][prev_y][ROOM_GROUND_FLOOR];

            // Only reset elevation if the previous tile was a pushable block
            if(prev_tile == ROOM_TILE_PUSHABLE_BLOCK ||
               prev_tile == ROOM_TILE_WALL){
                global_active_room->elevation[prev_x][prev_y] = global_active_room->original_elevation[prev_x][prev_y];
            }

            //update the previous position
            player.previous_x = original_x;
            player.previous_y = original_y;

            //reset the movement timer
            player.move_timer = 0.0f;          
        }

        // Handle reset key
        if (IsKeyPressed(KEY_R) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP)) {
            game_Reset();
        }
    }
}

void lower_Block_Elevation(int x, int y, int new_elevation) {
    // Ensure the new elevation is not negative
    if (new_elevation < 0) {
        new_elevation = 0;
    }

    int grid_x = x; 
    int grid_y = y; 

    // Check if the coordinates are valid within the grid
    if (grid_x >= 0 && grid_x < CONSTANT_GRID_WIDTH && grid_y >= 0 && grid_y < CONSTANT_GRID_HEIGHT) {
        // Update the elevation of the block at the given position
        global_active_room->elevation[grid_x][grid_y] = new_elevation;
    }
}

void player_Draw(int x, int y) {
    Rectangle source = 
    {
        0,0,
        player.texture.width * player.direction,
        player.texture.height
    };
    
    if (x == player.x && y == player.y) {
        Vector2 player_screen_pos = map_Grid_To_Screen(player.x, player.y, global_active_room->elevation[x][y]);
        DrawTextureRec(player.texture, source, (Vector2){player_screen_pos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)player_screen_pos.y} , WHITE);
        if(player.is_invincible)
        {
            DrawTexture(player.texture, player_screen_pos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)player_screen_pos.y , RED);
        }
    }
    
}

void player_Move_Player_And_Pushable_Block(int targetPlayerX, int targetPlayerY, int dirX, int dirY) {

    int targetBlockX = targetPlayerX + dirX;
    int targetBlockY = targetPlayerY + dirY;

    if (map_Can_Move_To_Position(targetPlayerX, targetPlayerY)) {
        int currentTile = global_active_room->layout[targetPlayerX][targetPlayerY][ROOM_GROUND_FLOOR];    
        int targetOriginalElevation = global_active_room->original_elevation[targetPlayerX][targetPlayerY];

        // Check if the player is trying to push a block
        if (currentTile == ROOM_TILE_PUSHABLE_BLOCK) {
            // Ensure the block can be moved
            if (player_Can_Move_Pushable_Block(targetBlockX, targetBlockY)) {
                // Move the block
                global_active_room->layout[targetBlockX][targetBlockY][ROOM_GROUND_FLOOR] = ROOM_TILE_PUSHABLE_BLOCK;
                global_active_room->layout[targetPlayerX][targetPlayerY][ROOM_GROUND_FLOOR] = ROOM_TILE_FLOOR; // Clear the old block position

                // Reset the elevation of the old block position to its original value
                global_active_room->elevation[targetPlayerX][targetPlayerY] = targetOriginalElevation;

                // Update the player's position
                player.x = targetPlayerX;
                player.y = targetPlayerY;

                // Maintain the player's elevation at the target position
                player.elevation = targetOriginalElevation;
                global_active_room->elevation[player.x][player.y] = player.elevation;

            } else {
                player.x = targetPlayerX;
                player.y = targetPlayerY;

                global_active_room->elevation[player.x][player.y] = targetOriginalElevation + 1;
                player.elevation = global_active_room->elevation[player.x][player.y];
            }
        }else if (currentTile == ROOM_TILE_WALL){
            player.x = targetPlayerX;
            player.y = targetPlayerY;

            global_active_room->elevation[player.x][player.y] = targetOriginalElevation + 1;
            player.elevation = global_active_room->elevation[player.x][player.y];
        }
        else 
        {
            player.x = targetPlayerX;
            player.y = targetPlayerY;
            player.elevation = targetOriginalElevation;
            global_active_room->elevation[player.x][player.y] = player.elevation;
        }
    }
}

bool player_Can_Move_Pushable_Block(int target_block_x, int target_block_y) {
    if (target_block_x < 0 || target_block_x >= CONSTANT_GRID_WIDTH || target_block_y < 0 || target_block_y >= CONSTANT_GRID_HEIGHT) {
        return false;
    }

    //prevent block from moving if the player will move to it from above
    int player_elevation = global_active_room->elevation[player.x][player.y];
    if(player_elevation > global_active_room->elevation[target_block_x][target_block_y])
    {
        return false;
    }

    int targetBlockType = global_active_room->layout[target_block_x][target_block_y][ROOM_GROUND_FLOOR];
    if (targetBlockType == ROOM_TILE_FLOOR) {
        return true;
    } else if (targetBlockType == ROOM_TILE_WALL || targetBlockType == ROOM_TILE_GEM || targetBlockType == ROOM_TILE_TREE || targetBlockType == ROOM_TILE_DOOR || targetBlockType == ROOM_TILE_ELEVATION_1 || targetBlockType == ROOM_TILE_ELEVATION_2) {
        return false;
    }

    return false;
}
