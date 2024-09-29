#ifndef ENEMY_INC_ENEMY_H
#define ENEMY_INC_ENEMY_H

#include "../../Direction.h"
#include "raylib.h"

typedef struct Enemy
{
    Rectangle hurtbox;
    float     left_bound;
    float     right_bound;
    float     patrol_velocity;
    Color     color;
    Direction direction;
} Enemy;

Enemy build_enemy(Rectangle hurtbox, float left_bound, float right_bound,
                  Color color, float patrol_velocity);
void  update_enemy(Enemy* enemy);

#endif /* ENEMY_H */