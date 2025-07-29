#ifndef MAP_H_
#define MAP_H_

#include "raylib.h"
#include "rooms.h"

Vector2 map_Cartesian_To_Isometric(Vector2 cartPt);
Vector2 map_Isometric_To_Cartesian(Vector2 isoPt);

Vector2 map_Grid_To_Screen(int x, int y, int elevation);
void    map_Draw_Ground_Floor(int x, int y);
void    map_Draw_First_Floor(int x, int y);
bool    map_Can_Move_To_Position(int x, int y);
void    map_Draw(int x, int y);

#endif
