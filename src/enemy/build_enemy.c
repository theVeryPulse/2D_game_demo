#include "inc/enemy.h"

/**
 * @brief Builds an enemy that patrols horizontally between left and right bound
 * 
 * @param hurtbox Initial position and size of hurtbox.
 * @param left_bound Left boundary of patrol.
 * @param right_bound Right boundary of patrol.
 * @param color Color of the enemy box.
 * @param patrol_velocity Patrol velocity.
 * @return `Enemy` struct
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
