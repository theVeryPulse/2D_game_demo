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

#endif /* APPLE_H */