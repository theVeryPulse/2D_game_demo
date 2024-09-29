#include "inc/apple.h"

/**
 * @brief Draws a progress bar in up-left corner.
 * 
 * @param apple_count Sum of collected and uncollected apples.
 * @param collected_apple_count Number of collected apples.
 */
void draw_apple_progress(int apple_count, int collected_apple_count)
{
    for (int i = 0; i < apple_count; ++i)
        DrawRectangleLines(i * 60 + 50, 50, 40, 40, GREEN);
    for (int i = 0; i < collected_apple_count; ++i)
        DrawRectangle(i * 60 + 50, 50, 40, 40, GREEN);
}
