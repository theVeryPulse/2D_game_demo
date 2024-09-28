#include "inc/player.h"

/**
 * @brief Handle player jumps: `W`, `Up-Arrow`, `SPACE`
 * 
 * @param player Pointer to player struct.
 */
void handle_player_jump(Player* player)
{
    if (player->jumps_left > 0
        && (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)
            || IsKeyPressed(KEY_W)))
    {
        player->position.y -= 2; // lift player up from possible floor
        player->velocity.y = player->jump_velocity;
        player->is_in_air = true;
        --(player->jumps_left);
    }
}
