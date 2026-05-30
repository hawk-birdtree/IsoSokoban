#ifndef PLAYER_H_
#define PLAYER_H_

#include "raylib.h"

typedef struct {
	int elevation;
	int previous_elevation;
    int hp;
    int max_hp;
    int x;
    int y;
	int previous_x;
	int previous_y;
	int target_block_x;
	int target_block_y;
    int direction;
    int i_frame_counter;
	float move_timer;
	Texture2D texture;
	bool has_won;
	bool has_lost;
	bool has_key;
    bool is_invincible;
}Player;

extern Player player;

void player_Init(void);
void player_Update(void);
void player_Activate_Elevation(int blockX, int blockY);
bool player_Can_Move_Pushable_Block(int targetBlockX, int targetBlockY);
void player_Move_Player_And_Pushable_Block(int targetPlayerX, int targetPlayerY, int dirX, int dirY);
void player_Draw(int x, int y);
void lower_Block_Elevation(int x, int y, int new_elevation);

#endif
