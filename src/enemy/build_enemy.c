#include "inc/enemy.h"

/**
 * @brief
 *
 * @return Enemy
 */
Enemy build_enemy(Rectangle hurtbox, float left_bound, float right_bound,
                  Color color, float patrol_velocity)
{
    Enemy enemy;
    enemy.hurtbox = hurtbox;
    enemy.left_bound = left_bound;
    enemy.right_bound = right_bound;
    enemy.color = color;
    enemy.patrol_velocity = patrol_velocity;
    enemy.direction = Right;
    return enemy;
}
