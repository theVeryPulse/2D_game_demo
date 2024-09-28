#include "inc/player.h"

/**
 * @brief Builds an initialized player struct
 *
 * @param pos_x player initial x position.
 * @param pos_y player initial y position.
 * @return Player struct.
 */
Player build_player(float pos_x, float pos_y)
{
    Player player = {0};
    player.position = (Vector2){.x = pos_x, .y = pos_y};
    player.velocity = (Vector2){.x = 0.0f, .y = 0.0f};
    player.acceleration = 0.7f;
    player.acceleration_air = 0.1f;
    player.max_x_speed = 5;
    player.max_y_speed = 5;
    player.jump_velocity = -20.0f;
    player.max_jumps = 2;
    player.jumps_left = player.max_jumps;
    player.is_in_air = true;
    player.texture = LoadTexture("resources/scarfy.png");
    player.frame_rectangle = (Rectangle){0.0f, 0.0f,
                                         (float)player.texture.width / 6,
                                         (float)player.texture.height};
    player.direction = Right;
    player.frame_rate = 8;
    player.frame_index = 0;
    player.frame_counter = 0;
    return player;
}
