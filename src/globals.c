#include "raylib.h"
#include "map_tiles.h"
#include "rooms.h"
#include "player.h"
#include "globals.h"
#include "raymath.h"
#include "textures.h"

Camera2D global_camera;
int      global_gems = 0;
float    global_delta_time = 0.0f;
int      global_current_room = 1;
Room    *global_active_room = &room_1;

void global_Init_Camera(void) {
	global_camera.target   = (Vector2){0.0f, 0.0f};
	global_camera.offset   = (Vector2){0.0f, (float)CONSTANT_SCREEN_HEIGHT/3.0f};
	global_camera.rotation = 0.0f;
	global_camera.zoom     = 0.9f;
}

void global_Camera_Update(void) {
    Vector2 playerScreenPos = map_Grid_To_Screen(player.x, player.y, global_active_room->elevation[(int)player.x][(int)player.y]);

    float lerpSpeed = 0.02f;  // 3;

    // Use Lerp to smoothly move the camera towards the target position
    global_camera.target.x = Lerp(global_camera.target.x, playerScreenPos.x, lerpSpeed);
    global_camera.target.y = Lerp(global_camera.target.y, playerScreenPos.y, lerpSpeed);
}


