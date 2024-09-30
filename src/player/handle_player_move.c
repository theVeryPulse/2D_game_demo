#include "inc/player.h"
#include <math.h> // fabs()

#define FPS 60

static void update_texture_frame(Player* player);

/**
 * @brief Handles player horizontal move: `<-`, `->`, `A`, `D`. Updates player
 *        texture frame.
 * 
 * @param player Pointer to player struct.
 * @param screen_width For stoping player from going out the screen.
 * @note player struct may be modified.
 */
void handle_player_move(Player* player, int screen_width)
{
    if (player->position.x < screen_width
        && (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)))
    {
        if (player->frame_rectangle.width < 0)
            player->frame_rectangle.width *= -1;
        if (player->velocity.x < player->max_x_speed)
        {
            if (player->is_in_air)
                player->velocity.x += player->acceleration_air;
            else
                player->velocity.x += player->acceleration;
        }
        ++(player->frame_counter);
        player->direction = Right;
        player->is_in_air = true;
    }
    else if (player->position.x > 0
             && (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)))
    {
        if (player->frame_rectangle.width > 0)
            player->frame_rectangle.width *= -1;
        if (-player->velocity.x < player->max_x_speed)
        {
            if (player->is_in_air)
                player->velocity.x -= player->acceleration_air;
            else
                player->velocity.x -= player->acceleration;
        }
        ++(player->frame_counter);
        player->direction = Left;
        player->is_in_air = true;
    }
    else if (!player->is_in_air)
    {
        if (fabs(player->velocity.x) < player->acceleration)
            player->velocity.x = 0;
        else if (player->velocity.x > 0)
            player->velocity.x -= player->acceleration;
        else if (player->velocity.x < 0)
            player->velocity.x += player->acceleration;
    }
    update_texture_frame(player);
}

static void update_texture_frame(Player* player)
{
    if (player->frame_counter >= FPS / player->frame_rate)
    {
        player->frame_counter = 0;
        ++(player->frame_index);
        if (player->frame_index > 5)
            player->frame_index = 0;
        player->frame_rectangle.x = (float)player->frame_index
                                    * (float)player->texture.width / 6;
    }
}
