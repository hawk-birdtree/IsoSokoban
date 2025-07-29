#include "enemies.h"
#include "globals.h"
#include "rooms.h"
#include "player.h"
#include "map_tiles.h"
#include "textures.h"
#include "math.h"

float enemy_x[GLOBAL_MAX_ENEMIES];
float enemy_y[GLOBAL_MAX_ENEMIES];
float enemy_initial_x[GLOBAL_MAX_ENEMIES]; // Example initial positions (you can change these)
float enemy_initial_y[GLOBAL_MAX_ENEMIES];
bool  enemy_is_moving;
float enemy_move_timer;
int   enemy_speed;

enum EnemyType enemyTypes[GLOBAL_MAX_ENEMIES] = {
    ENEMY_TYPE_RUNNER,
    ENEMY_TYPE_CHASER,
    ENEMY_TYPE_SPIKE,
    ENEMY_TYPE_RANDOM_WALKER,
    ENEMY_TYPE_HORIZONTAL,
    ENEMY_TYPE_VERTICAL,
    ENEMY_TYPE_HORIZONTAL_SNIPER,
};

void enemy_Init()
{
    enemy_is_moving = true;
    enemy_speed = GLOBAL_ENEMY_SPEED;

    for(int i = 0; i <GLOBAL_MAX_ENEMIES; i++)
    {
        enemy_move_timer = 0.0f;
    }

    // Save initial enemy positions for resetting
    for (int i = 0; i < GLOBAL_MAX_ENEMIES; i++) {
        enemy_initial_x[i] = enemy_x[i];
        enemy_initial_y[i] = enemy_y[i];
    }

    // Initialize enemy positions
    for (int i = 0; i < GLOBAL_MAX_ENEMIES; i++) {
        do {
            enemy_x[i] = GetRandomValue(1, GLOBAL_GRID_WIDTH - 1);
            enemy_y[i] = GetRandomValue(1, GLOBAL_GRID_HEIGHT - 1);
        } while (!map_Can_Move_To_Position(enemy_x[i], enemy_y[i]) ||
        enemy_Is_Position_Occupied(enemy_x[i], enemy_y[i], i) ||
        (enemy_x[i] == room_1.key_x     && enemy_y[i] == room_1.key_y) || // Avoid spawning on key
        (enemy_x[i] == room_1.door_x    && enemy_y[i] == room_1.door_y) || //void spawning on door
        (enemy_x[i] == player.x && enemy_y[i] == player.y)); // Avoid spawning on player
    }
}

void enemy_Update_Position() {
    glob_delta_time = GetFrameTime();  // Get the time passed since the last frame

    for (int i = 0; i < GLOBAL_MAX_ENEMIES; i++) {

        // Get the enemy type
        enum EnemyType enemyType = enemyTypes[i];

        // Calculate the direction from the enemy to the player
        int deltaX = player.x - enemy_x[i];
        int deltaY = player.y - enemy_y[i];

        // Calculate the distance to the player
        float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);

        // Update enemy movement based on a move timer for each enemy
        enemy_move_timer += glob_delta_time;  // Track time for this enemy

        // If the enemy's move timer has passed the cooldown threshold, move the enemy
        if (enemy_move_timer >= GLOBAL_ENEMY_MOVE_COOLDOWN) {
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

                        if (!enemy_Is_Position_Occupied(targetX, targetY, i) &&
                            map_Can_Move_To_Position(targetX, targetY) &&
                            room_1.layout[targetX][targetY][ROOM_GROUND_FLOOR] != ROOM_TILE_PUSHABLE_BLOCK) {
                                enemy_x[i] = targetX;
                                enemy_y[i] = targetY;
                            }
                    } else {
                        // Random walker behavior
                        static int moveCounter = 0; // Tracks how many tiles the enemy has moved
                        int randomDir;
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


                // case ENEMY_TYPE_CHASER:
                // {
                //     float playerDistanceX = player.x - enemy_x[i];
                //     float playerDistanceY = player.y - enemy_y[i];
                //     float distanceThreshold = 5.0f;
                //
                //     if (fabs(playerDistanceX) <= distanceThreshold && fabs(playerDistanceY) <= distanceThreshold) {
                //         int dirX = 0;
                //         int dirY = 0;
                //
                //         if (fabs(playerDistanceX) > fabs(playerDistanceY)) {
                //             dirX = (playerDistanceX < 0) ? -1 : 1; // Move left or right
                //         } else {
                //             dirY = (playerDistanceY < 0) ? -1 : 1; // Move up or down
                //         }
                //
                //         // Move the enemy towards the player
                //         int targetX = enemy_x[i] + dirX * enemy_speed;
                //         int targetY = enemy_y[i] + dirY * enemy_speed;
                //
                //         // Check if the target position is valid
                //         if (!enemy_Is_Position_Occupied(targetX, targetY, i) && map_Can_Move_To_Position(targetX, targetY) &&
                //             room_1.layout[targetX][targetY][ROOM_GROUND_FLOOR] != ROOM_TILE_PUSHABLE_BLOCK) {
                //             enemy_x[i] = targetX;
                //         enemy_y[i] = targetY;
                //             }
                //     }
                // }
                // break;

                case ENEMY_TYPE_RANDOM_WALKER:
                {
                    static int moveCounter = 0;  // Tracks how many tiles the enemy has moved
                    int randomDir;
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

                // case ENEMY_TYPE_CRAZY_WALKER:
                // {
                //     int randomDir;
                //     int randomValue = GetRandomValue(0, 4);
                //
                //     int targetX = enemy_x[i];
                //     int targetY = enemy_y[i];
                //
                //     if (randomValue < 3) {
                //         randomDir = 0;
                //     } else {
                //         randomDir = GetRandomValue(1, 4);  // Random movement direction
                //     }
                //
                //     if (randomDir != 0) {
                //         switch (randomDir) {
                //             case 1: // Up
                //                 targetY -= enemy_speed;
                //                 break;
                //             case 2: // Left
                //                 targetX -= enemy_speed;
                //                 break;
                //             case 3: // Down
                //                 targetY += enemy_speed;
                //                 break;
                //             case 4: // Right
                //                 targetX += enemy_speed;
                //                 break;
                //             default:
                //                 break;
                //         }
                //
                //         // Check if the target position is valid
                //         if (!Enemy_Is_Position_Occupied(targetX, targetY, i) &&
                //             Map_Can_Move_To(targetX, targetY) &&
                //             room_1.layout[targetX][targetY][ROOM_GROUND_FLOOR] != ROOM_TILE_PUSHABLE_BLOCK) {
                //             enemy_x[i] = targetX;
                //         enemy_y[i] = targetY;
                //             }
                //     }
                // }
                // break;

                case ENEMY_TYPE_HORIZONTAL:
                {
                    static int direction = 1;

                    int targetX = enemy_x[i] + (enemy_speed * direction);
                    int targetY = enemy_y[i];

                    if (targetX >= 0 && targetX < GLOBAL_GRID_WIDTH) {
                        if (!enemy_Is_Position_Occupied(targetX, targetY, i) && map_Can_Move_To_Position(targetX, targetY) &&
                            room_1.layout[targetX][targetY][ROOM_GROUND_FLOOR] != ROOM_TILE_PUSHABLE_BLOCK) {
                            enemy_x[i] = targetX;
                            } else {
                                direction = -direction;  // Reverse direction if blocked
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

                    if (targetY >= 0 && targetY < GLOBAL_GRID_HEIGHT) {
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
                    if (targetX >= 0 && targetX < GLOBAL_GRID_WIDTH) {
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

    // Check for collisions with the player for all enemies
    for (int i = 0; i < GLOBAL_MAX_ENEMIES; i++) {
        if (player.x == enemy_x[i] && player.y == enemy_y[i]) {
            room_Init_Ground_Floor(&room_1); // this should go to room 2 after picking up key in room 1
            room_Init_First_Floor(&room_1, 0, 0);
            enemy_Reset_Positions(); // Reset enemy positions if needed
            // Reset any other relevant variables here

            // Update player position to the initial position in the room
            player.x = room_1.start_x;
            player.y = room_1.start_y;
            glob_camera.target.x = player.x;
            glob_camera.target.y = player.y;
            glob_gems = 0;
            player.has_key = false;
        }
    }
}

void enemy_Draw(int x, int y)
{
    for (int i = 0; i < GLOBAL_MAX_ENEMIES; i++) {
        if (x == enemy_x[i] && y == enemy_y[i]) {
            // Vector2 enemyScreenPos = map_Grid_To_Screen(enemy_x[i], enemy_y[i], room_1.layout[(int)enemy_x[i]][(int)enemy_y[i]][ROOM_GROUND_FLOOR]);
             Vector2 enemyScreenPos = map_Grid_To_Screen(enemy_x[i], enemy_y[i], room_1.elevation[x][y]);

            switch (enemyTypes[i]) {
                case ENEMY_TYPE_RUNNER:
                    DrawTexture(texture_enemy_runner, enemyScreenPos.x + (float)GLOBAL_SCREEN_WIDTH / 2 , enemyScreenPos.y , WHITE);
                    break;
                case ENEMY_TYPE_CHASER:
                    DrawTexture(texture_enemy_chaser, enemyScreenPos.x + (float)GLOBAL_SCREEN_WIDTH / 2 , enemyScreenPos.y , WHITE);
                    break;
                case ENEMY_TYPE_SPIKE:
                    DrawTexture(texture_enemy_spike, enemyScreenPos.x + (float)GLOBAL_SCREEN_WIDTH / 2 , enemyScreenPos.y , WHITE);
                    break;
                case ENEMY_TYPE_RANDOM_WALKER:
                    DrawTexture(texture_enemy_random_walker, enemyScreenPos.x + (float)GLOBAL_SCREEN_WIDTH / 2 , enemyScreenPos.y , WHITE);
                    break;
                case ENEMY_TYPE_HORIZONTAL:
                    DrawTexture(texture_enemy_horizontal, enemyScreenPos.x + (float)GLOBAL_SCREEN_WIDTH / 2 , enemyScreenPos.y , WHITE);
                    break;
                case ENEMY_TYPE_VERTICAL:
                    DrawTexture(texture_enemy_vertical, enemyScreenPos.x + (float)GLOBAL_SCREEN_WIDTH / 2 , enemyScreenPos.y , WHITE);
                    break;
                case ENEMY_TYPE_HORIZONTAL_SNIPER:
                    DrawTexture(texture_enemy_sniper, enemyScreenPos.x + (float)GLOBAL_SCREEN_WIDTH / 2 , enemyScreenPos.y , WHITE);
                    break;
                default:
                    break;
            }
        }
    }
}

void enemy_Reset_Positions() {

    // Initialize enemy positions
    for (int i = 0; i < GLOBAL_MAX_ENEMIES; i++) {
        do {
            enemy_x[i] = GetRandomValue(1, GLOBAL_GRID_WIDTH - 1);
            enemy_y[i] = GetRandomValue(1, GLOBAL_GRID_HEIGHT - 1);
        } while (!map_Can_Move_To_Position(enemy_x[i], enemy_y[i]) ||
        enemy_Is_Position_Occupied(enemy_x[i], enemy_y[i], i) ||
        (enemy_x[i] == room_1.key_x && enemy_y[i] == room_1.key_y) || // Avoid spawning on key
        (enemy_x[i] == room_1.door_x && enemy_y[i] == room_1.door_y) ||
        (enemy_x[i] == player.x && enemy_y[i] == player.y)); // Avoid spawning on door
    }
}

bool enemy_Is_Position_Occupied(int x, int y, int currentEnemyIndex) {
    for (int i = 0; i < GLOBAL_MAX_ENEMIES; i++) {
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
