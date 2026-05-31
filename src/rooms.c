// why is the message for the door displayed at 0, 0?
//   the door message needs to be in the same place the door is at
// why can the push blocks get moved to elevated positions, but not the player?
// why does room 2 reset after the player get's all the things?
// why do the enemies reset to random positions after they collide with the player?
// why can the player move under some block but not others?
// why can the player push the push block into enemies, but not gems or keys?


#include "raylib.h"
#include "enemies.h"
#include "rooms.h"
#include "globals.h"
#include "player.h"
#include "stdio.h"

Room room_1;
Room room_2;

void room_Load_From_png(Room* room, const char* floor_0_path, const char* floor_1_path, const char* floor_2_path)
{
    Image floor_0 = LoadImage(floor_0_path);
    if(floor_0.data == NULL)
    {
        TraceLog(LOG_ERROR, "Failed to load image: %s", floor_0_path);
        return;
    }

    Image floor_1 = LoadImage(floor_1_path);
    if(floor_1.data == NULL)
    {
        TraceLog(LOG_ERROR, "Failed to load image: %s", floor_1_path);
        return;
    }
    
    Image floor_2 = LoadImage(floor_2_path);
    if(floor_2.data == NULL)
    {
        TraceLog(LOG_ERROR, "Failed to load image: %s", floor_2_path);
        return;
    }
    
    room->enemy_spawn_count = 0;

    //ground floor
    for(int x = 0; x < CONSTANT_GRID_WIDTH; x++)
    {
        for(int y = 0; y < CONSTANT_GRID_HEIGHT; y++)
        {
            Color pixel = GetImageColor(floor_0, x, y);
            
            if(ColorIsEqual(pixel, WHITE))
            {
                room->layout[x][y][ROOM_GROUND_FLOOR] = ROOM_TILE_FLOOR;
            }
            else if(ColorIsEqual(pixel, BLACK))
            {
                room->layout[x][y][ROOM_GROUND_FLOOR] = ROOM_TILE_WALL;
            }
            else if(ColorIsEqual(pixel, GREEN))
            {
                room->layout[x][y][ROOM_GROUND_FLOOR] = ROOM_TILE_TREE;
            }
            else if(ColorIsEqual(pixel, SKYBLUE))
            {
                room->layout[x][y][ROOM_GROUND_FLOOR] = ROOM_TILE_WATER;
            }
            else if(ColorIsEqual(pixel, BEIGE))
            {
                room->layout[x][y][ROOM_GROUND_FLOOR] = ROOM_TILE_KEY;
            }
            else if(ColorIsEqual(pixel, DARKGREEN))
            {
                room->layout[x][y][ROOM_GROUND_FLOOR] = ROOM_TILE_DOOR;
            }
            else if(ColorIsEqual(pixel, BROWN))
            {
                room->layout[x][y][ROOM_GROUND_FLOOR] = ROOM_TILE_GEM;
            }
            else if(ColorIsEqual(pixel, DARKPURPLE))
            {
                room->layout[x][y][ROOM_GROUND_FLOOR] = ROOM_TILE_PUSHABLE_BLOCK;
            }
            else if(ColorIsEqual(pixel, DARKBROWN))
            {
                room->layout[x][y][ROOM_GROUND_FLOOR] = ROOM_TILE_PUSHABLE_BLOCK_END_POINT;
            }
            else if(ColorIsEqual(pixel, LIGHTGRAY))
            {
                room->layout[x][y][ROOM_GROUND_FLOOR] = ROOM_TILE_ELEVATION_1;
                room->elevation[x][y] = ROOM_FIRST_FLOOR;
            }
            else if(ColorIsEqual(pixel, GRAY))
            {
                room->layout[x][y][ROOM_GROUND_FLOOR] = ROOM_TILE_ELEVATION_2;
                room->elevation[x][y] = ROOM_SECOND_FLOOR;
            }
            else if(ColorIsEqual(pixel, YELLOW))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_RUNNER;                
                    room->enemy_spawn_count++;
                }
            }
            else if(ColorIsEqual(pixel, GOLD))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_CHASER;                
                    room->enemy_spawn_count++;
                }               
            }
            else if(ColorIsEqual(pixel, MAROON))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_SPIKE;                
                    room->enemy_spawn_count++;
                }               
            }
            else if(ColorIsEqual(pixel,ORANGE))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_RANDOM_WALKER;                
                    room->enemy_spawn_count++;
                }               
            }
            else if(ColorIsEqual(pixel, PINK))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_HORIZONTAL;                
                    room->enemy_spawn_count++;
                }                
            }
            else if(ColorIsEqual(pixel, RED))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_VERTICAL;                
                    room->enemy_spawn_count++;
                }               
            }
            else if(ColorIsEqual(pixel, LIME))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_HORIZONTAL_SNIPER;                
                    room->enemy_spawn_count++;
                }               
            }
            else if(ColorIsEqual(pixel, BLUE))
            {
                room->start_x = x;
                room->start_y = y;
            }
        }
    }
    
    //first floor
    for(int x = 0; x < CONSTANT_GRID_WIDTH; x++)
    {
        for(int y = 0; y < CONSTANT_GRID_HEIGHT; y++)
        {
            Color pixel = GetImageColor(floor_1, x, y);
            
            if(ColorIsEqual(pixel, WHITE))
            {
                room->layout[x][y][ROOM_FIRST_FLOOR] = ROOM_TILE_FLOOR;
            }
            else if(ColorIsEqual(pixel, BLACK))
            {
                room->layout[x][y][ROOM_FIRST_FLOOR] = ROOM_TILE_WALL;
            }
            else if(ColorIsEqual(pixel, GREEN))
            {
                room->layout[x][y][ROOM_FIRST_FLOOR] = ROOM_TILE_TREE;
            }
            else if(ColorIsEqual(pixel, SKYBLUE))
            {
                room->layout[x][y][ROOM_FIRST_FLOOR] = ROOM_TILE_WATER;
            }
            else if(ColorIsEqual(pixel, BEIGE))
            {
                room->layout[x][y][ROOM_FIRST_FLOOR] = ROOM_TILE_KEY;
            }
            else if(ColorIsEqual(pixel, DARKGREEN))
            {
                room->layout[x][y][ROOM_FIRST_FLOOR] = ROOM_TILE_DOOR;
            }
            else if(ColorIsEqual(pixel, BROWN))
            {
                room->layout[x][y][ROOM_FIRST_FLOOR] = ROOM_TILE_GEM;
            }
            else if(ColorIsEqual(pixel, DARKPURPLE))
            {
                room->layout[x][y][ROOM_FIRST_FLOOR] = ROOM_TILE_PUSHABLE_BLOCK;
            }
            else if(ColorIsEqual(pixel, DARKBROWN))
            {
                room->layout[x][y][ROOM_FIRST_FLOOR] = ROOM_TILE_PUSHABLE_BLOCK_END_POINT;
            }
            else if(ColorIsEqual(pixel, LIGHTGRAY))
            {
                room->layout[x][y][ROOM_FIRST_FLOOR] = ROOM_TILE_ELEVATION_1;
                room->elevation[x][y] = ROOM_FIRST_FLOOR;
            }
            else if(ColorIsEqual(pixel, GRAY))
            {
                room->layout[x][y][ROOM_FIRST_FLOOR] = ROOM_TILE_ELEVATION_2;
                room->elevation[x][y] = ROOM_SECOND_FLOOR;
            }
            else if(ColorIsEqual(pixel, YELLOW))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_RUNNER;                
                    room->enemy_spawn_count++;
                }
            }
            else if(ColorIsEqual(pixel, GOLD))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_CHASER;                
                    room->enemy_spawn_count++;
                }               
            }
            else if(ColorIsEqual(pixel, MAROON))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_SPIKE;                
                    room->enemy_spawn_count++;
                }               
            }
            else if(ColorIsEqual(pixel,ORANGE))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_RANDOM_WALKER;                
                    room->enemy_spawn_count++;
                }               
            }
            else if(ColorIsEqual(pixel, PINK))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_HORIZONTAL;                
                    room->enemy_spawn_count++;
                }                
            }
            else if(ColorIsEqual(pixel, RED))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_VERTICAL;                
                    room->enemy_spawn_count++;
                }               
            }
            else if(ColorIsEqual(pixel, LIME))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_HORIZONTAL_SNIPER;                
                    room->enemy_spawn_count++;
                }               
            }
            else if(ColorIsEqual(pixel, BLUE))
            {
                room->start_x = x;
                room->start_y = y;
            }
        }
    }
    
    //second floor
    for(int x = 0; x < CONSTANT_GRID_WIDTH; x++)
    {
        for(int y = 0; y < CONSTANT_GRID_HEIGHT; y++)
        {
            Color pixel = GetImageColor(floor_2, x, y);
            
            if(ColorIsEqual(pixel, WHITE))
            {
                room->layout[x][y][ROOM_SECOND_FLOOR] = ROOM_TILE_FLOOR;
            }
            else if(ColorIsEqual(pixel, BLACK))
            {
                room->layout[x][y][ROOM_SECOND_FLOOR] = ROOM_TILE_WALL;
            }
            else if(ColorIsEqual(pixel, GREEN))
            {
                room->layout[x][y][ROOM_SECOND_FLOOR] = ROOM_TILE_TREE;
            }
            else if(ColorIsEqual(pixel, SKYBLUE))
            {
                room->layout[x][y][ROOM_SECOND_FLOOR] = ROOM_TILE_WATER;
            }
            else if(ColorIsEqual(pixel, BEIGE))
            {
                room->layout[x][y][ROOM_SECOND_FLOOR] = ROOM_TILE_KEY;
            }
            else if(ColorIsEqual(pixel, DARKGREEN))
            {
                room->layout[x][y][ROOM_SECOND_FLOOR] = ROOM_TILE_DOOR;
            }
            else if(ColorIsEqual(pixel, BROWN))
            {
                room->layout[x][y][ROOM_SECOND_FLOOR] = ROOM_TILE_GEM;
            }
            else if(ColorIsEqual(pixel, DARKPURPLE))
            {
                room->layout[x][y][ROOM_SECOND_FLOOR] = ROOM_TILE_PUSHABLE_BLOCK;
            }
            else if(ColorIsEqual(pixel, DARKBROWN))
            {
                room->layout[x][y][ROOM_SECOND_FLOOR] = ROOM_TILE_PUSHABLE_BLOCK_END_POINT;
            }
            else if(ColorIsEqual(pixel, LIGHTGRAY))
            {
                room->layout[x][y][ROOM_SECOND_FLOOR] = ROOM_TILE_ELEVATION_1;
                room->elevation[x][y] = ROOM_SECOND_FLOOR;
            }
            else if(ColorIsEqual(pixel, GRAY))
            {
                room->layout[x][y][ROOM_SECOND_FLOOR] = ROOM_TILE_ELEVATION_2;
                room->elevation[x][y] = ROOM_SECOND_FLOOR;
            }
            else if(ColorIsEqual(pixel, YELLOW))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_RUNNER;                
                    room->enemy_spawn_count++;
                }
            }
            else if(ColorIsEqual(pixel, GOLD))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_CHASER;                
                    room->enemy_spawn_count++;
                }               
            }
            else if(ColorIsEqual(pixel, MAROON))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_SPIKE;                
                    room->enemy_spawn_count++;
                }               
            }
            else if(ColorIsEqual(pixel,ORANGE))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_RANDOM_WALKER;                
                    room->enemy_spawn_count++;
                }               
            }
            else if(ColorIsEqual(pixel, PINK))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_HORIZONTAL;                
                    room->enemy_spawn_count++;
                }                
            }
            else if(ColorIsEqual(pixel, RED))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_VERTICAL;                
                    room->enemy_spawn_count++;
                }               
            }
            else if(ColorIsEqual(pixel, LIME))
            {
                if(room->enemy_spawn_count < CONSTANT_MAX_ENEMIES)
                {
                    room->enemy_spawn_x[room->enemy_spawn_count] = x;
                    room->enemy_spawn_y[room->enemy_spawn_count] = y;
                    room->enemy_spawn_type[room->enemy_spawn_count] = ENEMY_TYPE_HORIZONTAL_SNIPER;                
                    room->enemy_spawn_count++;
                }               
            }
            else if(ColorIsEqual(pixel, BLUE))
            {
                room->start_x = x;
                room->start_y = y;
            }
        }
    }
    
    //TO DO: more floors...?
    
    UnloadImage(floor_0);
    UnloadImage(floor_1);
    UnloadImage(floor_2);
}

void room_Pick_Up_Key(void) {
    // Change the tile type at the player's current position to a floor tile
    global_active_room->layout[(int)player.x][(int)player.y][ROOM_GROUND_FLOOR] = ROOM_TILE_FLOOR;
    player.has_key = true;
}

bool room_Can_Use_Door(void) {
    return player.has_key;
}

void room_Pick_Up_Loot(void){
    global_active_room->layout[(int)player.x][(int)player.y][ROOM_GROUND_FLOOR] = ROOM_TILE_FLOOR;
}
