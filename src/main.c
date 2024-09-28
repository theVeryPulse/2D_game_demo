#include "player/inc/player.h"
#include "collision/inc/collision.h"
#include "raylib.h"
#include <math.h>  // fabs()
#include <stdio.h> // printf()

#define FPS 60

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------
    const int screen_width = 1920;
    const int screen_height = 1080;

    InitWindow(screen_width, screen_height, "2D Game Demo");

    // Load resources / Initialize variables

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context
    // is required)

    Player player = build_player(screen_width / 2, screen_height / 2);

    const float gravity_acceleration = 0.8f;

    Rectangle floor = {
        .x = 0, .y = 700, .width = screen_width / 1.5, .height = 50};
    SetTargetFPS(FPS);
    //--------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // User Input and Update
        //----------------------------------------------------------------------

        if ((player.position.x < 0 && player.velocity.x < 0)
            || (player.position.x > screen_width && player.velocity.x > 0))
            player.velocity.x = 0;
        if (!aabb_collision(floor, get_player_hitbox(&player)))
            player.is_in_air = true;
        // Moves left/right
        if (player.position.x < screen_width
            && (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)))
        {
            if (player.frame_rectangle.width < 0)
                player.frame_rectangle.width = -player.frame_rectangle.width;
            if (player.velocity.x < player.max_x_speed)
            {
                if (player.is_in_air)
                    player.velocity.x += player.acceleration_air;
                else
                    player.velocity.x += player.acceleration;
            }
            ++(player.frame_counter);
            player.direction = Right;
        }
        else if (player.position.x > 0
                 && (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)))
        {
            if (player.frame_rectangle.width > 0)
                player.frame_rectangle.width = -player.frame_rectangle.width;
            if (-player.velocity.x < player.max_x_speed)
            {
                if (player.is_in_air)
                    player.velocity.x -= player.acceleration_air;
                else
                    player.velocity.x -= player.acceleration;
            }
            ++(player.frame_counter);
            player.direction = Left;
        }
        else if (!player.is_in_air)
        {
            if (fabs(player.velocity.x) < player.acceleration)
                player.velocity.x = 0;
            else if (player.velocity.x > 0)
                player.velocity.x -= player.acceleration;
            else if (player.velocity.x < 0)
                player.velocity.x += player.acceleration;
        }

        // Jump
        if (player.jumps_left > 0
            && (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)
                || IsKeyPressed(KEY_W)))
        {
            player.position.y -= 2; // move player away from floor
            player.velocity.y = player.jump_velocity;
            player.is_in_air = true;
            --(player.jumps_left);
        }

        // Falls while in air
        if (player.is_in_air)
            player.velocity.y += gravity_acceleration;
        // Stops falling at ground
        if (player.is_in_air
            && aabb_collision(floor, get_player_hitbox(&player)))
        {
            player.is_in_air = false;
            player.velocity.y = 0;
            player.position.y = floor.y + 1; // collide to prevent falling
            player.jumps_left = player.max_jumps;
            printf("Jumps reset.\n"); // test code
        }

        player.position.x += player.velocity.x;
        player.position.y += player.velocity.y;

        if (player.frame_counter >= FPS / player.frame_rate)
        {
            player.frame_counter = 0;
            ++(player.frame_index);
            if (player.frame_index > 5)
                player.frame_index = 0;
            player.frame_rectangle.x = (float)player.frame_index
                                       * (float)player.texture.width / 6;
        }
        //----------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------
        BeginDrawing();
        {
            /* Painter's Algorithm:
             * 1. Clear the screen
             * 2. Draw The Farthest Background
             * 3. Draw The Second Farthest Background
             * 4. Draw The Tile Map
             * 5. Draw The enemies and obstacles
             * 6. Draw The Player
             * 7. Display everything on screen
             */
            ClearBackground(RAYWHITE);
            DrawTexture(player.texture, 15, 40, WHITE);
            DrawRectangleLines(15, 40, player.texture.width,
                               player.texture.height, LIME);
            DrawRectangleLines(15 + (int)player.frame_rectangle.x,
                               40 + (int)player.frame_rectangle.y,
                               (int)player.frame_rectangle.width,
                               (int)player.frame_rectangle.height, RED);
            DrawRectangleRec(floor, GRAY);
            draw_player(&player);
        }
        EndDrawing();
        //----------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------
    UnloadTexture(player.texture); // Texture unloading
    CloseWindow();                 // Close window and OpenGL context
    //--------------------------------------------------------------------------

    return 0;
}
