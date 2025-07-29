#include "rooms.h"
#include "globals.h"
#include "player.h"
#include "stdio.h"

Room room_1;

void room_Init_Ground_Floor(Room* room) {
    glob_gems = 0;

    for (int x = 0; x < GLOBAL_GRID_WIDTH; ++x) {
        for (int y = 0; y < GLOBAL_GRID_HEIGHT; ++y) {

            room->layout[x][y][ROOM_GROUND_FLOOR] = ROOM_TILE_FLOOR;
            room->elevation[x][y]= ROOM_GROUND_FLOOR;

            if (room->elevation[x][y] > ROOM_NUMBER_OF_FLOORS) {
                // Print an error message to the console
                fprintf(stderr, "ERROR: ELEVATION OUT OF BOUNDS IN FILE: %s at line %d\n", __FILE__, __LINE__);

                // Optionally, you can also log this information to a file
                FILE *logFile = fopen("error_log.txt", "a");
                if (logFile) {
                    fprintf(logFile, "ERROR: ELEVATION OUT OF BOUNDS IN FILE: %s at line %d\n", __FILE__, __LINE__);
                    fclose(logFile);
                }

                // Close the game window and terminate the program
                CloseWindow();
            }
        }
    }
}

void room_Init_First_Floor(Room* room, int start_x, int start_y) {

    for (int x = 0; x < GLOBAL_GRID_WIDTH; ++x) {
        for (int y = 0; y < GLOBAL_GRID_HEIGHT; ++y) {
            if (room->elevation[x][y] > ROOM_NUMBER_OF_FLOORS) {
                // Print an error message to the console
                fprintf(stderr, "ERROR: ELEVATION OUT OF BOUNDS IN FILE: %s at line %d\n", __FILE__, __LINE__);

                // Optionally, you can also log this information to a file
                FILE *logFile = fopen("error_log.txt", "a");
                if (logFile) {
                    fprintf(logFile, "ERROR: ELEVATION OUT OF BOUNDS IN FILE: %s at line %d\n", __FILE__, __LINE__);
                    fclose(logFile);
                }
            }
        }
    }

    room->layout[0][7][ROOM_GROUND_FLOOR] = ROOM_TILE_ELEVATED_2;
    room->elevation[0][7]                 = ROOM_SECOND_FLOOR;
    room->layout[1][7][ROOM_GROUND_FLOOR] = ROOM_TILE_ELEVATED;
    room->elevation[1][7]                 = ROOM_FIRST_FLOOR;


    room->layout[7][4][ROOM_GROUND_FLOOR] = ROOM_TILE_ELEVATED_2;
    room->elevation[7][4]                 = ROOM_FIRST_FLOOR;
    room->layout[8][1][ROOM_GROUND_FLOOR] = ROOM_TILE_ELEVATED_2;
    room->elevation[8][1]                 = ROOM_FIRST_FLOOR;

    room->key_x = 8;
    room->key_y = 8;
    room->door_x = 4;
    room->door_y = 4;

    room->layout[room->door_x][room->door_y][ROOM_GROUND_FLOOR] = ROOM_TILE_DOOR;
    room->elevation[room->door_x][room->door_y]                 = ROOM_GROUND_FLOOR;
    room->layout[room->key_x][room->key_y][ROOM_GROUND_FLOOR]   = ROOM_TILE_KEY;
    room->elevation[room->key_x][room->key_y]                   = ROOM_GROUND_FLOOR;

    room->layout[2][8][ROOM_GROUND_FLOOR] = ROOM_TILE_WALL;
    room->layout[4][0][ROOM_GROUND_FLOOR] = ROOM_TILE_WALL;
    room->layout[6][4][ROOM_GROUND_FLOOR] = ROOM_TILE_WALL;


    room->layout[1][0][ROOM_GROUND_FLOOR] = ROOM_TILE_TREE;

    room->layout[9][6][ROOM_GROUND_FLOOR] = ROOM_TILE_GEM;
    room->layout[4][7][ROOM_GROUND_FLOOR] = ROOM_TILE_WATER;
    room->layout[3][2][ROOM_GROUND_FLOOR] = ROOM_TILE_WATER;
    room->layout[1][3][ROOM_GROUND_FLOOR] = ROOM_TILE_GEM;
/*
    int blockPosX[GLOBAL_MAX_PUSHBLOCKS] = {3,3,3,4,4,4,5,5,5};
    int blockPosY[GLOBAL_MAX_PUSHBLOCKS] = {4,5,6,4,5,6,4,5,6};*/

    int blockPosX[GLOBAL_MAX_PUSHBLOCKS] = {3,3,3};
    int blockPosY[GLOBAL_MAX_PUSHBLOCKS] = {4,5,5};

    for(int i = 0; i <GLOBAL_MAX_PUSHBLOCKS; i++)
    {
        room->block_X[i] = blockPosX[i];
        room->block_Y[i] = blockPosY[i];
    }

    for(int i = 0; i < GLOBAL_MAX_PUSHBLOCKS; i++)
    {
        room->layout[room->block_X[i]][room->block_Y[i]][ROOM_GROUND_FLOOR] = ROOM_TILE_PUSHABLE_BLOCK;
    }

    for (int x = 0; x < GLOBAL_GRID_WIDTH; ++x) {
        for (int y = 0; y < GLOBAL_GRID_HEIGHT; ++y) {
            room->original_elevation[x][y] = room_1.elevation[x][y];
        }
    }

    room->start_x = start_x;
    room->start_y = start_y;
    room->show_door_message = false;
}

void room_Pick_Up_Key(void) {
    // Change the tile type at the player's current position to a floor tile
    room_1.layout[(int)player.x][(int)player.y][ROOM_GROUND_FLOOR] = ROOM_TILE_FLOOR;
    player.has_key = true;
}

bool room_Can_Use_Door(void) {
    // Check if the player has the key
    return player.has_key;
}


void room_Pick_Up_Loot(void)
{
    room_1.layout[(int)player.x][(int)player.y][ROOM_GROUND_FLOOR] = ROOM_TILE_FLOOR;
}
