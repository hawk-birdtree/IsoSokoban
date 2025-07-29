#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

// NOTE: Gamepad name ID depends on drivers and OS
#define XBOX360_LEGACY_NAME_ID  "Xbox Controller"
#if defined(PLATFORM_RPI)
    #define XBOX360_NAME_ID     "Microsoft X-Box 360 pad"
    #define PS3_NAME_ID         "PLAYSTATION(R)3 Controller"
#else
    #define XBOX360_NAME_ID     "Xbox 360 Controller"
    #define PS3_NAME_ID         "PLAYSTATION(R)3 Controller"
#endif

#define GLOBAL_GRID_WIDTH            16
#define GLOBAL_GRID_HEIGHT           16
#define GLOBAL_MAX_PUSHBLOCKS         3
#define GLOBAL_MAX_GEMS               2
#define GLOBAL_TILE_SIZE             64
#define GLOBAL_SCREEN_WIDTH         800
#define GLOBAL_SCREEN_HEIGHT        480
#define GLOBAL_MAX_ENEMIES            7
#define GLOBAL_ENEMY_SPEED            0
#define GLOBAL_PLAYER_MOVE_COOLDOWN 0.2f
#define GLOBAL_ENEMY_MOVE_COOLDOWN  0.2f

extern Camera2D glob_camera;
extern int      glob_gems;
extern float    glob_delta_time;

void global_Init_Camera();
void global_Camera_Update();
void global_Draw_Debug_Info(void);
void global_Draw_Game_Messages(void);

#endif
