#include "inc/player.h"
#include "raylib.h"
#include <stdio.h>

static Vector2 get_texture_position(const Player* player);

/**
 * @brief Draws player in the window.
 * 
 * @param player Pointer to player sturct.
 */
void draw_player(const Player* player)
{
    DrawTextureRec(player->texture, player->frame_rectangle,
                    get_texture_position(player), WHITE);
    // Position indicator
    DrawCircle(player->position.x, player->position.y, 5, RED);
}

/**
 * @brief Get the position for rendering the texture for player.
 *
 * @param player Pointer to the player struct.
 * @return `Vector2`: position struct for texture.
 */
static Vector2 get_texture_position(const Player* player)
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
