#include "inc/player.h"

void respawn_player(Player* player, float x, float y)
{
    player->position = (Vector2){x, y};
    player->velocity = (Vector2){0, 0};
    player->status = Alive;
    player->is_in_air = true;
}
