#ifndef COLLISION_H
#define COLLISION_H

#include "../../player/inc/player.h"
#include "raylib.h"
#include <stdbool.h>

void solve_collision(Player* player, Rectangle object);
bool aabb_collision(Rectangle a, Rectangle b);

#endif /* COLLISION_H */
