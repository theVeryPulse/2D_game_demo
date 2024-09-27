/*
    WELCOME raylib EXAMPLES CONTRIBUTOR!

    This is a basic template to anyone ready to contribute with some code
   example for the library, here there are some guidelines on how to create an
   example to be included in raylib

    1. File naming: <module>_<description> - Lower case filename, words
   separated by underscore, no more than 3-4 words in total to describe the
   example. <module> referes to the primary raylib module the example is more
   related with (code, shapes, textures, models, shaders, raudio). i.e:
   core_input_multitouch, shapes_lines_bezier, shaders_palette_switch

    2. Follow below template structure, example info should list the module, the
   short description and the author of the example, twitter or github info could
   be also provided for the author. Short description should also be used on the
   title of the window.

    3. Code should be organized by sections:[Initialization]- [Update] - [Draw]
   - [De-Initialization] Place your code between the dotted lines for every
   section, please don't mix update logic with drawing and remember to unload
   all loaded resources.

    4. Code should follow raylib conventions:
   https://github.com/raysan5/raylib/wiki/raylib-coding-conventions Try to be
   very organized, using line-breaks appropiately.

    5. Add comments to the specific parts of code the example is focus on.
       Don't abuse with comments, try to be clear and impersonal on the
   comments.

    6. Try to keep the example simple, under 300 code lines if possible. Try to
   avoid external dependencies. Try to avoid defining functions outside the
   main(). Example should be as self-contained as possible.

    7. About external resources, they should be placed in a [resources] folder
   and those resources should be open and free for use and distribution. Avoid
   propietary content.

    8. Try to keep the example simple but with a creative touch.
       Simple but beautiful examples are more appealing to users!

    9. In case of additional information is required, just come to raylib
   Discord channel: example-contributions

    10. Have fun!

    The following files should be updated when adding a new example, it's
   planned to create some script to automatize this process but not available
   yet.

     - raylib/examples/<category>/<category>_example_name.c
     - raylib/examples/<category>/<category>_example_name.png
     - raylib/examples/<category>/resources/ *.*
     - raylib/examples/Makefile
     - raylib/examples/Makefile.Web
     - raylib/examples/README.md
     - raylib/projects/VS2022/examples/<category>_example_name.vcxproj
     - raylib/projects/VS2022/raylib.sln
     - raylib.com/common/examples.js
     - raylib.com/examples/<category>/<category>_example_name.html
     - raylib.com/examples/<category>/<category>_example_name.data
     - raylib.com/examples/<category>/<category>_example_name.wasm
     - raylib.com/examples/<category>/<category>_example_name.js
*/

#include "raylib.h"
#include <math.h>

#define FPS 60
#define MAX_FRAME_RATE 15
#define MIN_FRAME_RATE 1

#define SPRITE_MAX_SPEED 5

#define GROUND 600

typedef struct Player
{
    Vector2 position;
    Vector2 speed;
    float   acceleration;
    float   acceleration_air;
    float   jump_velocity;
} Player;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------
    const int screen_width  = 1920;
    const int screen_height = 1080;

    InitWindow(screen_width, screen_height, "2D Game Demo");

    // Load resources / Initialize variables

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context
    // is required)
    Texture2D scarfy          = LoadTexture("resources/scarfy.png");
    Rectangle frame_rectangle = {0.0f, 0.0f, (float)scarfy.width / 6,
                                 (float)scarfy.height};

    const int sprite_frame_rate    = 8;
    int       sprite_frame_index   = 0;
    int       sprite_frame_counter = 0;

    Vector2 sprite_position = {.x = screen_width / 2, .y = GROUND};
    Vector2 sprite_velocity = {.x = 0.0f, .y = 0.0f};

    const float sprite_acceleration     = 0.7f;
    const float sprite_acceleration_air = 0.1f;
    const float gravity_acceleration    = 0.8f;
    const float jump_velocity           = -20.0f;

    bool      sprite_in_air = false;
    const int max_jumps     = 2;
    int       jumps_left    = max_jumps;
    SetTargetFPS(FPS);
    //--------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------

        // Moves left/right
        if (sprite_position.x < screen_width - frame_rectangle.width
            && (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)))
        {
            if (frame_rectangle.width < 0)
                frame_rectangle.width = -frame_rectangle.width;
            if (sprite_velocity.x < SPRITE_MAX_SPEED)
            {
                if (sprite_in_air)
                    sprite_velocity.x += sprite_acceleration_air;
                else
                    sprite_velocity.x += sprite_acceleration;
            }
            ++sprite_frame_counter;
        }
        else if (sprite_position.x > 0
                 && (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)))
        {
            if (frame_rectangle.width > 0)
                frame_rectangle.width = -frame_rectangle.width;
            if (-sprite_velocity.x < SPRITE_MAX_SPEED)
            {
                if (sprite_in_air)
                    sprite_velocity.x -= sprite_acceleration_air;
                else
                    sprite_velocity.x -= sprite_acceleration;
            }
            ++sprite_frame_counter;
        }
        else if (fabs(sprite_velocity.x) < sprite_acceleration)
            sprite_velocity.x = 0;
        else if (sprite_velocity.x > 0)
            sprite_velocity.x -= sprite_acceleration;
        else if (sprite_velocity.x < 0)
            sprite_velocity.x += sprite_acceleration;

        // Jump
        if (jumps_left > 0
            && (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)
                || IsKeyPressed(KEY_W)))
        {
            sprite_velocity.y = jump_velocity;
            sprite_in_air     = true;
            --jumps_left;
        }

        // Falls while in air
        if (sprite_in_air)
            sprite_velocity.y += gravity_acceleration;
        // Stops falling at ground
        if (sprite_position.y > GROUND)
        {
            sprite_in_air     = false;
            sprite_velocity.y = 0;
            sprite_position.y = GROUND;
            jumps_left        = max_jumps;
        }

        sprite_position.x += sprite_velocity.x;
        sprite_position.y += sprite_velocity.y;

        if (sprite_frame_counter >= FPS / sprite_frame_rate)
        {
            sprite_frame_counter = 0;
            ++sprite_frame_index;
            if (sprite_frame_index > 5)
                sprite_frame_index = 0;
            frame_rectangle.x = (float)sprite_frame_index * (float)scarfy.width
                                / 6;
        }
        //----------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            // Draw everything:
            DrawTexture(scarfy, 15, 40, WHITE);
            DrawRectangleLines(15, 40, scarfy.width, scarfy.height, LIME);
            DrawRectangleLines(
                15 + (int)frame_rectangle.x, 40 + (int)frame_rectangle.y,
                (int)frame_rectangle.width, (int)frame_rectangle.height, RED);
            // Draw target frame of the sprite
            DrawTextureRec(scarfy, frame_rectangle, sprite_position, WHITE);
        }
        EndDrawing();
        //----------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------
    UnloadTexture(scarfy); // Texture unloading
    CloseWindow();         // Close window and OpenGL context
    //--------------------------------------------------------------------------

    return 0;
}
