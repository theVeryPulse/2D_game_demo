#include "inc/player.h"
#include <math.h> // fabs()

/**
 * @brief Handles player horizontal move: `<-`, `->`, `A`, `D`
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
            player->frame_rectangle.width = -player->frame_rectangle.width;
        if (player->velocity.x < player->max_x_speed)
        {
            if (player->is_in_air)
                player->velocity.x += player->acceleration_air;
            else
                player->velocity.x += player->acceleration;
        }
        ++(player->frame_counter);
        player->direction = Right;
    }
    else if (player->position.x > 0
             && (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)))
    {
        if (player->frame_rectangle.width > 0)
            player->frame_rectangle.width = -player->frame_rectangle.width;
        if (-player->velocity.x < player->max_x_speed)
        {
            if (player->is_in_air)
                player->velocity.x -= player->acceleration_air;
            else
                player->velocity.x -= player->acceleration;
        }
        ++(player->frame_counter);
        player->direction = Left;
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
}
