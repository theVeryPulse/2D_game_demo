#include "inc/apple.h"

void draw_apples(const Apple apples[], int apple_count)
{
    for (int i = 0; i < apple_count; ++i)
    {
        if (apples[i].status == NotCollected)
            DrawRectangleRec(apples[i].box, GREEN);
    }
}
