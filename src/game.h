#ifndef GAME_H_
#define GAME_H_

#include "rooms.h"

extern float game_timer; // Timer starts at 10 seconds
extern float game_maxTimer; // Maximum timer value

extern float game_rectWidth; // Starting width of the rectangle
extern float game_rectHeight; // Height of the rectangle
extern float game_rectX; // X position of the rectangle
extern float game_rectY; // Y position of the rectangle

void game_Init();
void game_Reset();
void game_Load_Next_Room(Room* next_room);
void game_Update();
void game_Draw();
void game_Unload();

#endif
