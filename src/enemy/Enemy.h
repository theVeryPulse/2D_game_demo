#ifndef ENEMY_H
#define ENEMY_H

#include "../Direction.h"
#include "raylib.h"

typedef struct Enemy
{
    Rectangle hurtbox;
    int       left_bound;
    int       right_bound;
    int       patrol_speed;
    Color     color;
    Direction direction;
} Enemy;

#endif /* ENEMY_H */
