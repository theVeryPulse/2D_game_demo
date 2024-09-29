#include "inc/enemy.h"

/**
 * @brief Makes enemy patrols between left and right boundaries.
 * 
 * @param enemy pointer to enemy struct.
 */
void update_enemy(Enemy* enemy)
{
    enemy->hurtbox.x += enemy->patrol_velocity;
    if (enemy->hurtbox.x > enemy->right_bound)
    {
        if (enemy->patrol_velocity > 0)
            enemy->patrol_velocity *= -1;
        enemy->hurtbox.x = enemy->right_bound;
    }
    else if (enemy->hurtbox.x < enemy->left_bound)
    {
        if (enemy->patrol_velocity < 0)
            enemy->patrol_velocity *= -1;
        enemy->hurtbox.x = enemy->left_bound;
    }
}
