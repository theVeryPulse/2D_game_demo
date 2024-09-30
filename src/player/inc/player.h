#ifndef PLAYER_H
#define PLAYER_H

#include "../../Direction.h"
#include "../../enemy/inc/enemy.h"
#include "raylib.h"

typedef enum Status
{
    Alive,
    Dead
} Status;

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
    int       frame_rate;
    int       frame_index;
    int       frame_counter;
    Status    status;
    int       respawn_countdown;
} Player;

Player    build_player(float pos_x, float pos_y);
void      draw_player(const Player* player);
Rectangle get_player_hitbox(const Player* player);
void      handle_player_move(Player* player, int screen_width);
void      handle_player_jump(Player* player);
void      respawn_player(Player* player, float x, float y);
void update_player(Player* player, const Rectangle objects[], int object_count,
                   const Enemy* enemy);

#endif