#include "inc/enemy.h"

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
