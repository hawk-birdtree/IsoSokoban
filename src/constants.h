#ifndef CONSTANTS_H_
#define CONSTANTS_H_

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

#define CONSTANT_GRID_WIDTH            16
#define CONSTANT_GRID_HEIGHT           16
#define CONSTANT_MAX_PUSHBLOCKS         3
#define CONSTANT_MAX_GEMS               2
#define CONSTANT_TILE_SIZE             64
#define CONSTANT_SCREEN_WIDTH         800
#define CONSTANT_SCREEN_HEIGHT        480
#define CONSTANT_MAX_ENEMIES            7
#define CONSTANT_ENEMY_SPEED            1
#define CONSTANT_PLAYER_MOVE_COOLDOWN 0.2f
#define CONSTANT_ENEMY_MOVE_COOLDOWN  0.2f

#endif