#include "textures.h"

Texture2D texture_floor;
Texture2D texture_wall;
Texture2D texture_tree;
Texture2D texture_water;
Texture2D texture_elevation;
Texture2D texture_elevation_2;
Texture2D texture_door;
Texture2D texture_key;
Texture2D texture_gem;
Texture2D texture_push;

Texture2D texture_enemy_runner;
Texture2D texture_enemy_chaser;
Texture2D texture_enemy_random_walker;
Texture2D texture_enemy;
Texture2D texture_enemy_spike;
Texture2D texture_enemy_horizontal;
Texture2D texture_enemy_vertical;
Texture2D texture_enemy_sniper;

void textures_Init(void)
{
    player.texture              = LoadTexture("../out/playerTile.png");

    texture_floor               = LoadTexture("../out/floorTile.png");
    texture_wall                = LoadTexture("../out/wallTile.png");
    texture_tree                = LoadTexture("../out/treeTile.png");
    texture_water               = LoadTexture("../out/waterTile.png");
    texture_elevation           = LoadTexture("../out/elevatedTile.png");
    texture_elevation_2         = LoadTexture("../out/elevatedTile.png");
    // texture_elevation           = LoadTexture("../out/breakTile.png");
    // texture_elevation_2         = LoadTexture("../out/blueTile.png");
    texture_door                = LoadTexture("../out/doorTile.png");
    texture_key                 = LoadTexture("../out/keyTile.png");
    texture_gem                 = LoadTexture("../out/gemTile.png");
    texture_push                = LoadTexture("../out/pushTile.png");
    texture_enemy_runner        = LoadTexture("../out/enemyRunner.png");
    texture_enemy_chaser        = LoadTexture("../out/Runner2.png");
    texture_enemy_random_walker = LoadTexture("../out/enemyRandomWalker.png");
    texture_enemy_spike         = LoadTexture("../out/enemySpike.png");
    texture_enemy_horizontal    = LoadTexture("../out/enemyStraightLine.png");
    texture_enemy_vertical      = LoadTexture("../out/enemyStraightLine_2.png");
    texture_enemy_sniper        = LoadTexture("../out/enemyHorizontal.png");
}

void texture_Unload(void)
{
    UnloadTexture(player.texture);

    UnloadTexture(texture_floor);
    UnloadTexture(texture_wall);
    UnloadTexture(texture_tree);
    UnloadTexture(texture_water);
    UnloadTexture(texture_elevation);
    UnloadTexture(texture_elevation_2);
    UnloadTexture(texture_door);
    UnloadTexture(texture_key);
    UnloadTexture(texture_gem);
    UnloadTexture(texture_push);
    UnloadTexture(texture_enemy_runner);
    UnloadTexture(texture_enemy_chaser);
    UnloadTexture(texture_enemy_random_walker);
    UnloadTexture(texture_enemy_spike);
    UnloadTexture(texture_enemy_horizontal);
    UnloadTexture(texture_enemy_vertical);
    UnloadTexture(texture_enemy_sniper);
}
