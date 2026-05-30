#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "raylib.h"
#include "constants.h"
#include "rooms.h"

extern Camera2D glob_camera;
extern int      glob_gems;
extern float    glob_delta_time;
extern int      glob_current_room;
extern Room    *glob_active_room;

void global_Init_Camera();
void global_Camera_Update();
void global_Draw_Game_Messages(void);

#endif
