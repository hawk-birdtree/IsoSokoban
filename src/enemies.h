#ifndef ENEMIES_H_
#define ENEMIES_H_

#include "globals.h"

enum EnemyType {
    ENEMY_TYPE_RUNNER = 0, // The current enemy type
    ENEMY_TYPE_CHASER,
    ENEMY_TYPE_SPIKE,
    ENEMY_TYPE_RANDOM_WALKER,
    ENEMY_TYPE_HORIZONTAL,
    ENEMY_TYPE_VERTICAL,
	ENEMY_TYPE_HORIZONTAL_SNIPER,
};

extern enum EnemyType enemy_types[GLOBAL_MAX_ENEMIES];

extern float enemy_x[GLOBAL_MAX_ENEMIES];
extern float enemy_y[GLOBAL_MAX_ENEMIES];
extern float enemy_initial_x[GLOBAL_MAX_ENEMIES]; // Example initial positions (you can change these)
extern float enemy_initial_y[GLOBAL_MAX_ENEMIES];
extern bool  enemy_is_moving;
extern float enemy_move_timer;
extern int   enemy_speed;

void enemy_Init();
void enemy_Reset_Positions();
void enemy_Update_Position();
void enemy_Draw(int x, int y);
bool enemy_Is_Position_Occupied(int x, int y, int currentEnemyIndex);

#endif
