#ifndef APPLE_H
#define APPLE_H

#include "raylib.h"

typedef enum AppleStatus
{
    NotCollected,
    Collected
} AppleStatus;

typedef struct Apple
{
    AppleStatus status;
    Rectangle   box;
} Apple;

void draw_apples(const Apple apples[], int apple_count);
void draw_apple_progress(int apple_count, int collected_apple_count);

#endif /* APPLE_H */