#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

typedef enum Direction
{
    UndefinedDirection,
    Left,
    Right,
} Direction;

typedef struct Player
{
    Vector2   position;
    Vector2   velocity;
    float     acceleration;
    float     acceleration_air;
    float     max_x_speed;
    float     max_y_speed;
    float     jump_velocity;
    int       max_jumps;
    int       jumps_left;
    bool      is_in_air;
    Texture2D texture;
    Rectangle frame_rectangle;
    Direction direction;
} Player;

Player    build_player(float pos_x, float pos_y);
void      draw_player(const Player* player);
Rectangle get_player_hitbox(const Player* player);

#endif