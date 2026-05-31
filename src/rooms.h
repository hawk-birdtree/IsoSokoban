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
    ROOM_TILE_PUSHABLE_BLOCK_END_POINT,
	ROOM_TILE_ELEVATION_1,
	ROOM_TILE_ELEVATION_2,
} Room_Tile_Type;

typedef enum {
	ROOM_GROUND_FLOOR = 0,
	ROOM_FIRST_FLOOR,
	ROOM_SECOND_FLOOR,
} Room_Floor_Elevation;

typedef struct {
    int layout[CONSTANT_GRID_WIDTH][CONSTANT_GRID_HEIGHT][ROOM_NUMBER_OF_FLOORS];
	int elevation[CONSTANT_GRID_WIDTH][CONSTANT_GRID_HEIGHT];
	int original_elevation[CONSTANT_GRID_WIDTH][CONSTANT_GRID_HEIGHT];
    int start_x;
    int start_y;
	int key_x;
	int key_y;
	int door_x;
	int door_y;
	int block_X[CONSTANT_MAX_PUSHBLOCKS];
	int block_Y[CONSTANT_MAX_PUSHBLOCKS];
	bool show_door_message;
    int enemy_spawn_x[CONSTANT_MAX_ENEMIES];
    int enemy_spawn_y[CONSTANT_MAX_ENEMIES];
    int enemy_spawn_type[CONSTANT_MAX_ENEMIES];
    int enemy_spawn_count;
} Room;

extern Room room_1;
extern Room room_2;

void room_Init_Ground_Floor(Room* room);
void room_Init_First_Floor(Room* room, int startX, int startY);
void room_Load_From_png(Room* room, const char* floor_0_path, const char* floor_1_path, const char* floor_2_path);
void room_Pick_Up_Key(void);
bool room_Can_Use_Door(void);
void room_Pick_Up_Loot(void);

#endif
