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

#define GROUND 720

typedef struct Player
{
    Vector2 position;
    Vector2 velocity;
    float   acceleration;
    float   acceleration_air;
    float   max_x_speed;
    float   max_y_speed;
    float   jump_velocity;
    int     max_jumps;
    int     jumps_left;
    bool    is_in_air;
} Player;

Player build_player(float pos_x, float pos_y)
{
    Player player;
    player.position = (Vector2){.x = pos_x, .y = pos_y};
    player.velocity = (Vector2){.x = 0.0f, .y = 0.0f};
    player.acceleration = 0.7f;
    player.acceleration_air = 0.1f;
    player.max_x_speed = 5;
    player.max_y_speed = 5;
    player.jump_velocity = -20.0f;
    player.max_jumps = 2;
    player.jumps_left = player.max_jumps;
    player.is_in_air = false;
    return player;
}

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
    Texture2D scarfy = LoadTexture("resources/scarfy.png");
    Rectangle frame_rectangle = {0.0f, 0.0f, (float)scarfy.width / 6,
                                 (float)scarfy.height};

    const int sprite_frame_rate = 8;
    int       sprite_frame_index = 0;
    int       sprite_frame_counter = 0;

    Player player = build_player(screen_width / 2, GROUND);

    const float gravity_acceleration = 0.8f;

    SetTargetFPS(FPS);
    //--------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------

        // Moves left/right
        if (player.position.x < screen_width - frame_rectangle.width
            && (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)))
        {
            if (frame_rectangle.width < 0)
                frame_rectangle.width = -frame_rectangle.width;
            if (player.velocity.x < player.max_x_speed)
            {
                if (player.is_in_air)
                    player.velocity.x += player.acceleration_air;
                else
                    player.velocity.x += player.acceleration;
            }
            ++sprite_frame_counter;
        }
        else if (player.position.x > 0
                 && (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)))
        {
            if (frame_rectangle.width > 0)
                frame_rectangle.width = -frame_rectangle.width;
            if (-player.velocity.x < player.max_x_speed)
            {
                if (player.is_in_air)
                    player.velocity.x -= player.acceleration_air;
                else
                    player.velocity.x -= player.acceleration;
            }
            ++sprite_frame_counter;
        }
        else if (fabs(player.velocity.x) < player.acceleration)
            player.velocity.x = 0;
        else if (player.velocity.x > 0)
            player.velocity.x -= player.acceleration;
        else if (player.velocity.x < 0)
            player.velocity.x += player.acceleration;

        // Jump
        if (player.jumps_left > 0
            && (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)
                || IsKeyPressed(KEY_W)))
        {
            player.velocity.y = player.jump_velocity;
            player.is_in_air = true;
            --(player.jumps_left);
        }

        // Falls while in air
        if (player.is_in_air)
            player.velocity.y += gravity_acceleration;
        // Stops falling at ground
        if (player.position.y > GROUND)
        {
            player.is_in_air = false;
            player.velocity.y = 0;
            player.position.y = GROUND;
            player.jumps_left = player.max_jumps;
        }

        player.position.x += player.velocity.x;
        player.position.y += player.velocity.y;

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
            DrawTextureRec(scarfy, frame_rectangle, player.position, WHITE);
            DrawRectangle(0, GROUND, screen_width, 20, GRAY);
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
