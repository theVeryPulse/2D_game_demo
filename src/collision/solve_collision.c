#include "inc/collision.h"
#include <stdio.h> // test printf()

static Vector2 get_overlap(Rectangle a, Rectangle b);

/**
 * @brief Solves collisiong with shallow axis method.
 * 
 * @param player Pointer to player struct.
 * @param object Object to check collision.
 */
void solve_collision(Player* player, Rectangle object)
{
    Rectangle hitbox = get_player_hitbox(player);
    Vector2   overlap = get_overlap(hitbox, object);

    // printf("overlap: x: %f, y: %f\n", overlap.x, overlap.y); // test code
    if (overlap.x > overlap.y) // y is the shallow axis
    {
        if (player->velocity.y > 0) // Player lands on object
        {
            player->position.y = object.y + 1;
            player->jumps_left = player->max_jumps;
            player->is_in_air = false;
            // printf("Jumps reset.\n"); // test code
        }
        else if (player->velocity.y < 0) // Player bumps on object
            player->position.y = object.y + object.height;
        else // Player standing on object
        {
        }
        player->velocity.y = 0;
    }
    else
    {
        if (player->velocity.x > 0)
            player->position.x = object.x - hitbox.width / 2 + 1;
        else
            player->position.x = object.x + object.width ;
        player->velocity.x = 0;
    }
}

/**
 * @brief Calculates the overlap bewteen two rectaangles on x and y axis.
 * 
 * @param a Rectangle a
 * @param b Rectangle b
 * @return `Vector2` 
 */
static Vector2 get_overlap(Rectangle a, Rectangle b)
{
    Vector2 overlap;
    if (a.x < b.x)
        overlap.x = a.x + a.width - b.x;
    else
        overlap.x = b.x + b.width - a.x;

    if (a.y < b.y)
        overlap.y = a.y + a.height - b.y;
    else
        overlap.y = b.y + b.height - a.y;

    if (overlap.x < 0)
        overlap.x = 0;
    if (overlap.y < 0)
        overlap.y = 0;
    return overlap;
}
