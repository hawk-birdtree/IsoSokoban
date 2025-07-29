#ifndef PLAYER_H_
#define PLAYER_H_

#include "raylib.h"

typedef struct {
	int elevation;
	int previous_elevation;
    float x;
    float y;
	float previous_x;
	float previous_y;
	float target_block_x;
	float target_block_y;
	float move_timer;
	Texture2D texture;
	bool has_won;
	bool has_lost;
	bool has_key;
}Player;

extern Player player;

void player_Init(void);
void player_Update(void);
void player_Activate_Elevation(int blockX, int blockY);
bool player_Can_Move_Pushable_Block(int targetBlockX, int targetBlockY);
void player_Move_Player_And_Pushable_Block(int targetPlayerX, int targetPlayerY, int dirX, int dirY);
void player_Draw(int x, int y);
void player_Reset(void);
void lower_Block_Elevation(float x, float y, float new_elevation);

#endif
