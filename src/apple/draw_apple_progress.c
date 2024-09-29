#include "inc/apple.h"

void draw_apple_progress(int apple_count, int collected_apple_count)
{
    for (int i = 0; i < apple_count; ++i)
        DrawRectangleLines(i * 50 + 10, 10, 40, 40, GREEN);
    for (int i = 0; i < collected_apple_count; ++i)
        DrawRectangle(i * 50 + 10, 10, 40, 40, GREEN);
}
