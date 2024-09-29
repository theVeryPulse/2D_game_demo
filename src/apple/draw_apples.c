#include "inc/apple.h"

/**
 * @brief Draws all uncollected apples on screen.
 * 
 * @param apples Pointer to apple array.
 * @param apple_count Number of apples in the array.
 */
void draw_apples(const Apple apples[], int apple_count)
{
    for (int i = 0; i < apple_count; ++i)
    {
        if (apples[i].status == NotCollected)
            DrawRectangleRec(apples[i].box, GREEN);
    }
}
