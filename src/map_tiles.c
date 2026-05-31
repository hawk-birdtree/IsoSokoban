#include "map_tiles.h"
#include "globals.h"
#include "raylib.h"
#include "rooms.h"
#include "textures.h"
#include "stdlib.h"
#include "stdio.h"

Vector2 map_Cartesian_To_Isometric(Vector2 cartPt) {
	Vector2 tempPt;
	tempPt.x = cartPt.x - cartPt.y;
	tempPt.y = (cartPt.x + cartPt.y) / 2;
	return tempPt;
}

Vector2 map_Isometric_To_Cartesian(Vector2 isoPt) {
	Vector2 tempPt;
	tempPt.x = (2 * isoPt.y + isoPt.x) / 2;
	tempPt.y = (2 * isoPt.y - isoPt.x) / 2;
	return tempPt;
}

Vector2 map_Grid_To_Screen(int x, int y, int elevation) {
	Vector2 screenPos;

	screenPos.x = (x - y) * ((float)CONSTANT_TILE_SIZE / 2);
	screenPos.y = (x + y) * ((float)CONSTANT_TILE_SIZE / 4) - elevation * ((float)CONSTANT_TILE_SIZE / 2);
	return screenPos;
}

bool map_Can_Move_To_Position(int x, int y) {
	if (x < 0 || x >= CONSTANT_GRID_WIDTH || y < 0 || y >= CONSTANT_GRID_HEIGHT) {
		return false; // Out of bounds
	}

	int tileType = global_active_room->layout[x][y][ROOM_GROUND_FLOOR];
	int elevation = global_active_room->elevation[x][y];
	int currentElevation = global_active_room->elevation[(int)player.x][(int)player.y];

	if(tileType == ROOM_TILE_TREE || tileType == ROOM_TILE_WATER){
		return false;
	}

	// Disallow movement onto impassable tiles, unless the player is at a higher elevation
	if (tileType == ROOM_TILE_WALL) {
		int originalElevation = global_active_room->original_elevation[x][y];

		if(currentElevation > originalElevation){
				return(true);
			}
		return(false); // Still disallow movement onto impassable tiles otherwise
	}

	// Allow standing on pushable blocks
	if (tileType == ROOM_TILE_PUSHABLE_BLOCK) {
		return true; // Always allowed to stand on blocks
	}

	// Allow movement between tiles of the same or adjacent elevation
	if (abs(currentElevation - elevation) <= 1) {
		return true;
	}

	// Allow falling to the ground level if elevated over 1 height
	if (currentElevation > 0 && elevation == 0) {
		return true;
	}

	return false; // Otherwise, movement is not allowed
}

static int map_elevation_1 = 1;

void map_Draw_Ground_Floor(int x, int y)
{
	Vector2 screenPos = map_Grid_To_Screen(x, y, global_active_room->layout[x][y][map_elevation_1]);
	DrawTexture(texture_floor, 	  (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)screenPos.y , WHITE);
}

void map_Draw_First_Floor(int x, int y)
{
	Vector2 screenPos = map_Grid_To_Screen(x, y, global_active_room->layout[x][y][map_elevation_1]);
	int elevation = global_active_room->elevation[x][y];

	if (elevation > ROOM_NUMBER_OF_FLOORS) {
		TraceLog(LOG_ERROR, "ELEVATION OUT OF BOUNDS IN FILE: %s at line %d", __FILE__, __LINE__);
	}

	int text_size = 20;
	Color tint = WHITE;

	switch (global_active_room->layout[x][y][ROOM_GROUND_FLOOR]) {
		case ROOM_TILE_WALL:
			DrawTexture(texture_wall, 	  (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)screenPos.y , WHITE);
			DrawText(TextFormat("%d, %d \n%d", x, y,  global_active_room->elevation[x][y]), (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)screenPos.y , text_size, tint);
			break;
		case ROOM_TILE_TREE:
			DrawTexture(texture_tree,  	  (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)screenPos.y , WHITE);
			//DrawText(TextFormat("%d, %d \n%d", x, y,  global_active_room->elevation[x][y]), (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)screenPos.y , text_size, tint);
			break;
		case ROOM_TILE_WATER:
			DrawTexture(texture_water, 	  (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)screenPos.y , WHITE);
			//DrawText(TextFormat("%d, %d \n%d", x, y,  global_active_room->elevation[x][y]), (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)screenPos.y , text_size, tint);
			break;
		case ROOM_TILE_DOOR:
			DrawTexture(texture_door, 	  (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)screenPos.y , WHITE);
			//DrawText(TextFormat("%d, %d \n%d", x, y,  global_active_room->elevation[x][y]), (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)screenPos.y , text_size, tint);
			break;
		case ROOM_TILE_GEM:
			DrawTexture(texture_gem, 	  (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)screenPos.y , WHITE);
			//DrawText(TextFormat("%d, %d \n%d", x, y,  global_active_room->elevation[x][y]), (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)screenPos.y , text_size, tint);
			break;
		case ROOM_TILE_PUSHABLE_BLOCK:
			DrawTexture(texture_push, 	  (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)screenPos.y , WHITE);
			// DrawText(TextFormat("%d, %d \n%d", x, y,  global_active_room->elevation[x][y]), (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)screenPos.y , text_size, tint);
			DrawText(TextFormat("%d", global_active_room->elevation[x][y]), (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f + 24, (float)screenPos.y + 12, text_size, tint);
			break;
		case ROOM_TILE_KEY:
			DrawTexture(texture_key,		  (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)screenPos.y , WHITE);
			//DrawText(TextFormat("%d, %d \n%d", x, y,  global_active_room->elevation[x][y]), (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)screenPos.y , text_size, tint);
			break;
		case ROOM_TILE_ELEVATION_1:
			DrawTexture(texture_elevation, 	  (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)screenPos.y , WHITE);
			//DrawText(TextFormat("%d, %d \n%d", x, y,  global_active_room->elevation[x][y]), (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)screenPos.y , text_size, tint);
			break;
		case ROOM_TILE_ELEVATION_2:
			DrawTexture(texture_elevation_2,  (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)screenPos.y , WHITE);
			//DrawText(TextFormat("%d, %d \n%d", x, y,  global_active_room->elevation[x][y]), (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)screenPos.y , text_size, tint);
			break;
		default:
			DrawTexture(texture_floor, 	  (float)screenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2.0f , (float)screenPos.y , WHITE);
			break;
	}
}

void map_Draw(int x, int y) {
	map_Draw_Ground_Floor(x,y);
	map_Draw_First_Floor(x,y);
}
