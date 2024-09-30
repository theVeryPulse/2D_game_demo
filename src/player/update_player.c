#include "inc/player.h"
#include "../collision/inc/collision.h" // aabb_collision()
#include <stdio.h>                      // printf()

#define FPS 60

/**
 * @brief Updates player position and solves collision. Updates texture frame.
 *        Checks enemy hit.
 *
 * @param player Pointer to player struct
 * @param objects Pointer to objects array
 * @param object_count Number of objects in array
 * @note
 * Method: Interleaving single-axis movement and collision detection
 */
void update_player(Player* player, const Rectangle objects[], int object_count)
{
    // Update x-axis and solve collision
    player->position.x += player->velocity.x;
    for (int i = 0; i < object_count; ++i)
    {
        Rectangle hitbox = get_player_hitbox(player);
        if (aabb_collision(objects[i], hitbox))
        {
            if (player->velocity.x > 0) // going right
                player->position.x = objects[i].x - hitbox.width / 2 - 0.1f;
            else if (player->velocity.x < 0) // going left
                player->position.x = objects[i].x + objects[i].width
                                     + hitbox.width / 2 + 0.1f;
            player->velocity.x = 0;
        }
    }

    // Update y-axis and solve collision
    player->position.y += player->velocity.y;
    for (int i = 0; i < object_count; ++i)
    {
        if (aabb_collision(objects[i], get_player_hitbox(player)))
        {
            if (player->velocity.y > 0)
            {
                // player lands on object
                player->position.y = objects[i].y;
                player->is_in_air = false;
                player->jumps_left = player->max_jumps;
                // printf("Jump reset\n");
            }
            else if (player->velocity.y < 0)
                player->position.y = objects[i].y + objects[i].height
                                     + get_player_hitbox(player).height;
            player->velocity.y = 0;
        }
    }
}
