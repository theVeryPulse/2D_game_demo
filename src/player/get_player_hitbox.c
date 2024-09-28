#include "inc/player.h"
#include "raylib.h"
#include <math.h> // fabs()

/**
 * @brief Builds the player hitbox struct
 *
 * @param player Pointer to player struct.
 * @return `Rectangle` player's hitbox.
 */
Rectangle get_player_hitbox(const Player* player)
{
    Rectangle hitbox;
    hitbox.width = fabs(player->frame_rectangle.width) / 2.5;
    hitbox.height = player->frame_rectangle.height;
    hitbox.y = player->position.y - player->frame_rectangle.height;
    hitbox.x = player->position.x - hitbox.width / 2;
    return hitbox;
}