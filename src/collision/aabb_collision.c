#include "raylib.h"
#include <stdbool.h>

/**
 * @brief Checks if two axis-aligned bounding box (AABB) collides.
 *
 * @param a Box a
 * @param b Box b
 * @return `true` if two boxes collides.
 */
bool aabb_collision(Rectangle a, Rectangle b)
{
    // clang-format off
    return (a.x < b.x + b.width) &&
           (a.x + a.width > b.x) &&
           (a.y < b.y + b.height) &&
           (a.y + a.height > b.y);
}
