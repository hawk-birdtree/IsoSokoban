#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "raylib.h"
#include "constants.h"
#include "rooms.h"

extern Camera2D global_camera;
extern int      global_gems;
extern float    global_delta_time;
extern int      global_current_room;
extern Room    *global_active_room;

void global_Init_Camera(void);
void global_Camera_Update(void);

#endif
