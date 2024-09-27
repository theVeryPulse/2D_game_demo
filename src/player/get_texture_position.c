#include "inc/player.h"
#include "raylib.h"
#include <stdio.h>

/**
 * @brief Get the position for rendering the texture for player.
 *
 * @param player Pointer to the player struct.
 * @return `Vector2`: position struct for texture.
 */
Vector2 get_texture_position(const Player* player)
{
    Vector2 position;
    if (player->direction == Right)
        position.x = player->position.x
                     - (float)player->frame_rectangle.width / 2;
    else if (player->direction == Left)
        position.x = player->position.x
                     + (float)player->frame_rectangle.width / 2;
    else
        printf("WARNING: get_texture_position: undefined direction.\n");
    position.y = player->position.y - player->frame_rectangle.height;
    return position;
}
