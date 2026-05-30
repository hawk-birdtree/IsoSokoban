#ifndef ROOMS_H_
#define ROOMS_H_

#include "constants.h"
#include <stdbool.h>

#define ROOM_NUMBER_OF_FLOORS 3

typedef enum {
	ROOM_TILE_FLOOR = 0,
	ROOM_TILE_WALL,
	ROOM_TILE_TREE,
	ROOM_TILE_WATER,
	ROOM_TILE_KEY,
	ROOM_TILE_DOOR,
	ROOM_TILE_GEM,
	ROOM_TILE_PUSHABLE_BLOCK,
	ROOM_TILE_ELEVATED,
	ROOM_TILE_ELEVATED_2,
} Room_Tile_Type;

typedef enum {
	ROOM_GROUND_FLOOR = 0,
	ROOM_FIRST_FLOOR,
	ROOM_SECOND_FLOOR,
} Room_Floor_Elevation;

typedef struct {
    int layout[GLOBAL_GRID_WIDTH][GLOBAL_GRID_HEIGHT][ROOM_NUMBER_OF_FLOORS];
	int elevation[GLOBAL_GRID_WIDTH][GLOBAL_GRID_HEIGHT];
	int original_elevation[GLOBAL_GRID_WIDTH][GLOBAL_GRID_HEIGHT];
    int start_x;
    int start_y;
	int key_x;
	int key_y;
	int door_x;
	int door_y;
	int block_X[GLOBAL_MAX_PUSHBLOCKS];
	int block_Y[GLOBAL_MAX_PUSHBLOCKS];
	bool show_door_message;
} Room;

extern Room room_1;
extern Room room_2;

void room_Init_Ground_Floor(Room* room);
void room_Init_First_Floor(Room* room, int startX, int startY);
void room_Pick_Up_Key(void);
bool room_Can_Use_Door(void);
void room_Pick_Up_Loot(void);

#endif
