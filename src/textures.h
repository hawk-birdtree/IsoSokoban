#ifndef TEXTURES_H_
#define TEXTURES_H_

#include "raylib.h"
#include "player.h"

extern Texture2D texture_floor;
extern Texture2D texture_wall;
extern Texture2D texture_tree;
extern Texture2D texture_water;
extern Texture2D texture_elevation;
extern Texture2D texture_elevation_2;
extern Texture2D texture_grass;
extern Texture2D texture_door;
extern Texture2D texture_key;
extern Texture2D texture_gem;
extern Texture2D texture_push;

extern Texture2D texture_enemy_runner;
extern Texture2D texture_enemy_chaser;
extern Texture2D texture_enemy_random_walker;
extern Texture2D texture_enemy_spike;
extern Texture2D texture_enemy_horizontal;
extern Texture2D texture_enemy_vertical;
extern Texture2D texture_enemy_sniper;

void textures_Init(void);
void texture_Unload(void);

#endif
