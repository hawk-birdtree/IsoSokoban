#include "enemies.h"
#include "globals.h"
#include "rooms.h"
#include "player.h"
#include "map_tiles.h"
#include "textures.h"
#include <math.h>
#include <stddef.h>

float enemy_x[CONSTANT_MAX_ENEMIES];
float enemy_y[CONSTANT_MAX_ENEMIES];
float enemy_initial_x[CONSTANT_MAX_ENEMIES]; // Example initial positions (you can change these)
float enemy_initial_y[CONSTANT_MAX_ENEMIES];
int   enemy_facing_direction[CONSTANT_MAX_ENEMIES];
bool  enemy_is_moving;
float enemy_move_timer;
int   enemy_speed;

enum EnemyType enemyTypes[CONSTANT_MAX_ENEMIES] = {
    ENEMY_TYPE_RUNNER,
    ENEMY_TYPE_CHASER,
    ENEMY_TYPE_SPIKE,
    ENEMY_TYPE_RANDOM_WALKER,
    ENEMY_TYPE_HORIZONTAL,
    ENEMY_TYPE_VERTICAL,
    ENEMY_TYPE_HORIZONTAL_SNIPER,
};

void enemy_Init(void)
{
    enemy_is_moving = true;
    enemy_speed = 0; //CONSTANT_ENEMY_SPEED;
    enemy_move_timer = 0.0f;

    // Initialize enemy positions
    for(int i = 0; i < global_active_room->enemy_spawn_count; i++)
    {
        enemy_x[i] = global_active_room->enemy_spawn_x[i];
        enemy_y[i] = global_active_room->enemy_spawn_y[i];
        enemyTypes[i] = global_active_room->enemy_spawn_type[i];
    }

    // Save initial enemy positions for resetting
    for (int i = 0; i < CONSTANT_MAX_ENEMIES; i++) {
        enemy_initial_x[i] = enemy_x[i];
        enemy_initial_y[i] = enemy_y[i];
        enemy_facing_direction[i] = 1;
    }

}

void enemy_Update_Position(void) {
    global_delta_time = GetFrameTime();  // Get the time passed since the last frame

    for (int i = 0; i < CONSTANT_MAX_ENEMIES; i++) {

        // Get the enemy type
        enum EnemyType enemyType = enemyTypes[i];

        // Calculate the direction from the enemy to the player
        int deltaX = player.x - enemy_x[i];
        int deltaY = player.y - enemy_y[i];

        // Calculate the distance to the player
        float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);

        // Update enemy movement based on a move timer for each enemy
        enemy_move_timer += global_delta_time;  // Track time for this enemy

        // If the enemy's move timer has passed the cooldown threshold, move the enemy
        if (enemy_move_timer >= CONSTANT_ENEMY_MOVE_COOLDOWN) {
            switch (enemyType) {
                case ENEMY_TYPE_RUNNER:
                {
                    if(distance != 0)
                    {
                        // Normalize the direction vector
                        int dirX = (int)(deltaX / distance);
                        int dirY = (int)(deltaY / distance);

                        // Move the enemy towards the player at the correct speed
                        int targetX = enemy_x[i] + dirX * enemy_speed;
                        int targetY = enemy_y[i] + dirY * enemy_speed;
                        
                        if(dirX != 0)
                        {
                            enemy_facing_direction[i] = dirX;
                        }

                        // Check if the target position is valid
                        if (!enemy_Is_Position_Occupied(targetX, targetY, i) && map_Can_Move_To_Position(targetX, targetY) &&
                            room_1.layout[targetX][targetY][ROOM_GROUND_FLOOR] != ROOM_TILE_PUSHABLE_BLOCK) {
                                enemy_x[i] = targetX;
                                enemy_y[i] = targetY;
                        }                        
                    }
                }
                break;

                case ENEMY_TYPE_CHASER:
                {
                    float playerDistanceX = player.x - enemy_x[i];
                    float playerDistanceY = player.y - enemy_y[i];
                    float distanceThreshold = 4.0f;

                    if (fabs(playerDistanceX) <= distanceThreshold && fabs(playerDistanceY) <= distanceThreshold) {
                        // Chaser behavior: move towards the player
                        int dirX = 0;
                        int dirY = 0;

                        if (fabs(playerDistanceX) > fabs(playerDistanceY)) {
                            dirX = (playerDistanceX < 0) ? -1 : 1; // Move left or right
                        } else {
                            dirY = (playerDistanceY < 0) ? -1 : 1; // Move up or down
                        }

                        int targetX = enemy_x[i] + dirX * enemy_speed;
                        int targetY = enemy_y[i] + dirY * enemy_speed;
                        
                        if(dirX != 0)
                        {
                            enemy_facing_direction[i] = dirX;
                        }

                        if (!enemy_Is_Position_Occupied(targetX, targetY, i) &&
                            map_Can_Move_To_Position(targetX, targetY) &&
                            room_1.layout[targetX][targetY][ROOM_GROUND_FLOOR] != ROOM_TILE_PUSHABLE_BLOCK) {
                                enemy_x[i] = targetX;
                                enemy_y[i] = targetY;
                            }
                    } else {
                        // Random walker behavior
                        static int moveCounter = 0; // Tracks how many tiles the enemy has moved
                        int randomDir = 0;
                        int randomValue = GetRandomValue(0, 10);

                        int targetX = enemy_x[i];
                        int targetY = enemy_y[i];

                        if (moveCounter < 3) { // Continue in the current direction
                            moveCounter++;
                        } else { // Pick a new random direction
                            moveCounter = 0;
                            if (randomValue < 3) {
                                randomDir = 0; // Stay in the same direction
                            } else {
                                randomDir = GetRandomValue(1, 4); // Choose a new direction (1=Up, 2=Left, 3=Down, 4=Right)
                            }
                        }

                        if (randomDir != 0) {
                            switch (randomDir) {
                                case 1: // Up
                                    targetY -= enemy_speed;
                                    break;
                                case 2: // Left
                                    targetX -= enemy_speed;
                                    break;
                                case 3: // Down
                                    targetY += enemy_speed;
                                    break;
                                case 4: // Right
                                    targetX += enemy_speed;
                                    break;
                                default:
                                    break;
                            }

                            if (!enemy_Is_Position_Occupied(targetX, targetY, i) &&
                                map_Can_Move_To_Position(targetX, targetY) &&
                                room_1.layout[targetX][targetY][ROOM_GROUND_FLOOR] != ROOM_TILE_PUSHABLE_BLOCK) {
                                    enemy_x[i] = targetX;
                                    enemy_y[i] = targetY;
                                }
                        }
                    }
                }
                break;

                case ENEMY_TYPE_RANDOM_WALKER:
                {
                    static int moveCounter = 0;  // Tracks how many tiles the enemy has moved
                    int randomDir = 0;
                    int randomValue = GetRandomValue(0, 4);

                    int targetX = enemy_x[i];
                    int targetY = enemy_y[i];

                    // Move several tiles before picking a new random direction
                    if (moveCounter < 3) {  // Move 3 tiles in the current direction before picking a new direction
                        moveCounter++;
                    } else {
                        moveCounter = 0;  // Reset the counter
                        if (randomValue < 3) {
                            randomDir = 0;  // Stay in the same direction
                        } else {
                            randomDir = GetRandomValue(1, 4);  // Pick a new random direction (1=Up, 2=Left, 3=Down, 4=Right)
                        }
                    }

                    if (randomDir != 0) {
                        switch (randomDir) {
                            case 1: // Up
                                targetY -= enemy_speed;
                                break;
                            case 2: // Left
                                targetX -= enemy_speed;
                                break;
                            case 3: // Down
                                targetY += enemy_speed;
                                break;
                            case 4: // Right
                                targetX += enemy_speed;
                                break;
                            default:
                                break;
                        }

                        // Check if the target position is valid
                        if (!enemy_Is_Position_Occupied(targetX, targetY, i) &&
                            map_Can_Move_To_Position(targetX, targetY) &&
                            room_1.layout[targetX][targetY][ROOM_GROUND_FLOOR] != ROOM_TILE_PUSHABLE_BLOCK) {
                                enemy_x[i] = targetX;
                                enemy_y[i] = targetY;
                            }
                    }
                }
                break;

                case ENEMY_TYPE_HORIZONTAL:
                {
                    static int direction = 1;

                    int targetX = enemy_x[i] + (enemy_speed * direction);
                    int targetY = enemy_y[i];

                    if (targetX >= 0 && targetX < CONSTANT_GRID_WIDTH) {
                        if (!enemy_Is_Position_Occupied(targetX, targetY, i) && map_Can_Move_To_Position(targetX, targetY) &&
                            room_1.layout[targetX][targetY][ROOM_GROUND_FLOOR] != ROOM_TILE_PUSHABLE_BLOCK) {
                            enemy_x[i] = targetX;
                            enemy_facing_direction[i] = direction;
                            } else {
                                direction = -direction;  // Reverse direction if blocked
                                enemy_facing_direction[i] = direction;
                            }
                    } else {
                        direction = -direction;  // Reverse if out of bounds
                    }
                }
                break;

                case ENEMY_TYPE_VERTICAL:
                {
                    static int direction = 1;

                    int targetX = enemy_x[i];
                    int targetY = enemy_y[i] + (enemy_speed * direction);

                    if (targetY >= 0 && targetY < CONSTANT_GRID_HEIGHT) {
                        if (!enemy_Is_Position_Occupied(targetX, targetY, i) && map_Can_Move_To_Position(targetX, targetY) &&
                            room_1.layout[targetX][targetY][ROOM_GROUND_FLOOR] != ROOM_TILE_PUSHABLE_BLOCK) {
                            enemy_y[i] = targetY;
                            } else {
                                direction = -direction;  // Reverse direction if blocked
                            }
                    } else {
                        direction = -direction;  // Reverse if out of bounds
                    }
                }
                break;

                case ENEMY_TYPE_HORIZONTAL_SNIPER:
                {
                    static int direction = 1;  // Direction of movement (1 for right, -1 for left)
                    static int startX = -1;   // Store the original X position
                    static int startY = -1;   // Store the original Y position

                    // Initialize starting position once
                    if (startX == -1 && startY == -1) {
                        startX = enemy_x[i];
                        startY = enemy_y[i];
                    }

                    int targetX = enemy_x[i] + (enemy_speed * direction);
                    int targetY = enemy_y[i];

                    // Check bounds and collisions
                    if (targetX >= 0 && targetX < CONSTANT_GRID_WIDTH) {
                        if (!enemy_Is_Position_Occupied(targetX, targetY, i) &&
                            map_Can_Move_To_Position(targetX, targetY) &&
                            room_1.layout[targetX][targetY][ROOM_GROUND_FLOOR] != ROOM_TILE_PUSHABLE_BLOCK) {
                            enemy_x[i] = targetX;  // Move enemy
                            } else {
                                // Collision detected, reset position to start and continue
                                enemy_x[i] = startX;
                                enemy_y[i] = startY;
                            }
                    } else {
                        // Out of bounds, reset position to start
                        enemy_x[i] = startX;
                        enemy_y[i] = startY;
                    }
                }
                break;

                default:
                    break;
            }

            enemy_move_timer = 0.0f;  // Reset the move timer for this enemy after moving
        }
    }
}

void enemy_Draw(int x, int y)
{
    Texture2D *tex = NULL;

    for (int i = 0; i < CONSTANT_MAX_ENEMIES; i++) {
        if (x == enemy_x[i] && y == enemy_y[i]) {

            switch (enemyTypes[i]) {
                case ENEMY_TYPE_RUNNER:         tex = &texture_enemy_runner;        break;
                case ENEMY_TYPE_CHASER:         tex = &texture_enemy_chaser;        break;
                case ENEMY_TYPE_SPIKE:          tex = &texture_enemy_spike;         break;
                case ENEMY_TYPE_RANDOM_WALKER:  tex = &texture_enemy_random_walker; break;
                case ENEMY_TYPE_HORIZONTAL:     tex = &texture_enemy_horizontal;    break;
                case ENEMY_TYPE_VERTICAL:       tex = &texture_enemy_vertical;      break;
                case ENEMY_TYPE_HORIZONTAL_SNIPER: tex = &texture_enemy_sniper;     break;
                default: break;
            }

            Vector2 enemyScreenPos = map_Grid_To_Screen(enemy_x[i], enemy_y[i], room_1.elevation[x][y]);
            if (tex) {
                Rectangle source = {0, 0, tex->width * enemy_facing_direction[i], tex->height};
                DrawTextureRec(*tex, source, (Vector2){enemyScreenPos.x + (float)CONSTANT_SCREEN_WIDTH / 2, enemyScreenPos.y}, WHITE);
            }
        }
    }
}

void enemy_Reset_Positions(void) {

    // Initialize enemy positions
    for (int i = 0; i < CONSTANT_MAX_ENEMIES; i++) {
        do {
            enemy_x[i] = GetRandomValue(1, CONSTANT_GRID_WIDTH - 1);
            enemy_y[i] = GetRandomValue(1, CONSTANT_GRID_HEIGHT - 1);
        } while (!map_Can_Move_To_Position(enemy_x[i], enemy_y[i]) ||
        enemy_Is_Position_Occupied(enemy_x[i], enemy_y[i], i) ||
        (enemy_x[i] == room_1.key_x && enemy_y[i] == room_1.key_y) || // Avoid spawning on key
        (enemy_x[i] == room_1.door_x && enemy_y[i] == room_1.door_y) ||
        (enemy_x[i] == player.x && enemy_y[i] == player.y)); // Avoid spawning on door
    }
}

bool enemy_Is_Position_Occupied(int x, int y, int currentEnemyIndex) {
    for (int i = 0; i < CONSTANT_MAX_ENEMIES; i++) {
        // Skip the current enemy being checked (currentEnemyIndex)
        if (i == currentEnemyIndex) {
            continue;
        }

        if ((x == enemy_x[i] && y == enemy_y[i]) || (room_1.layout[x][y][ROOM_GROUND_FLOOR] == ROOM_TILE_PUSHABLE_BLOCK)) {
            return true;
        }
    }
    return false;
}
