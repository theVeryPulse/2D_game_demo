#include "player/inc/player.h"
#include "collision/inc/collision.h"
#include "enemy/inc/enemy.h"
#include "apple/inc/apple.h"
#include "raylib.h"
#include <stdio.h> // printf()

#define FPS 60

static void draw_scene(const Player* player, const Rectangle objects[],
                       int object_count, const Enemy* enemy,
                       const Apple apples[], int apple_count,
                       int collected_apple_count, bool all_apples_collected);

static void handle_player_enemy_collision(Player* player, const Enemy* enemy);

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

    Player      player = build_player(100, 600);
    const float gravity_acceleration = 0.8f;

    // clang-format off
    Rectangle floor        = {.x = 0,    .y = screen_height - 200, .width = screen_width, .height = 200};
    Rectangle left_block   = {.x = 200,  .y = screen_height - 400, .width = 250,          .height = 200};
    Rectangle block_in_air = {.x = 700,  .y = 300,                 .width = 200,          .height = 100};
    Rectangle right_block  = {.x = 1200, .y = screen_height - 400, .width = 250,          .height = 200};
    // clang-format on

    Rectangle objects[] = {floor, left_block, right_block, block_in_air};
    Enemy enemy = build_enemy((Rectangle){.x = left_block.x + left_block.width,
                                          .y = floor.y - 100.0f,
                                          .height = 100.0f,
                                          .width = 100.0f},
                              left_block.x + left_block.width - 1,
                              right_block.x - 100 + 1, RED, 2.0f);

    // clang-format off
    Apple apples[] = {(Apple){NotCollected, (Rectangle){.x = 775,  .y = 250,          .height = 50, .width = 50}},
                      (Apple){NotCollected, (Rectangle){.x = 800,  .y = floor.y - 50, .height = 50, .width = 50}},
                      (Apple){NotCollected, (Rectangle){.x = 1700, .y = floor.y - 50, .height = 50, .width = 50}}};
    // clang-format on

    int  apple_count = sizeof(apples) / sizeof(apples[0]);
    int  object_count = sizeof(objects) / sizeof(objects[0]);
    bool all_apples_collected = false;
    int  collected_apple_count = 0;

    SetTargetFPS(FPS);
    //--------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Short pause before respawn if player runs into enemy
        // ---------------------------------------------------------------------
        if (player.respawn_countdown > 0)
        {
            draw_scene(&player, objects, object_count, &enemy, apples,
                       apple_count, collected_apple_count,
                       all_apples_collected);
            --(player.respawn_countdown);
            continue;
        }
        if (player.status == Dead)
            respawn_player(&player, 100, 600);
        // ---------------------------------------------------------------------

        // User Input
        //----------------------------------------------------------------------
        handle_player_move(&player, screen_width);
        handle_player_jump(&player);
        // ---------------------------------------------------------------------

        // Update
        // ---------------------------------------------------------------------
        update_enemy(&enemy);

        if ((player.position.x < 0 && player.velocity.x < 0)
            || (player.position.x > screen_width && player.velocity.x > 0))
            player.velocity.x = 0;
        if (player.is_in_air)
            player.velocity.y += gravity_acceleration;

        update_player(&player, objects, object_count);

        handle_player_enemy_collision(&player, &enemy);
        if (!all_apples_collected)
        {
            Rectangle hitbox = get_player_hitbox(&player);
            for (int i = 0; i < apple_count; ++i)
            {
                if (apples[i].status != Collected
                    && aabb_collision(apples[i].box, hitbox))
                {
                    apples[i].status = Collected;
                    ++collected_apple_count;
                }
            }
        }
        if (collected_apple_count == apple_count)
            all_apples_collected = true;
        //----------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------
        draw_scene(&player, objects, object_count, &enemy, apples, apple_count,
                   collected_apple_count, all_apples_collected);
        //----------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------
    UnloadTexture(player.texture); // Texture unloading
    CloseWindow();                 // Close window and OpenGL context
    //--------------------------------------------------------------------------

    return 0;
}

/**
 * @brief Draw the new frame for entire window
 *
 * @param player Pointer to player struct
 * @param objects Pointer to objects array
 * @param object_count Number of objects in the array
 * @param enemy Pointer to enemy struct
 * @note
 * Painter's Algorithm:
 * 1. Clear the screen
 * 2. Draw The Farthest Background
 * 3. Draw The Second Farthest Background
 * 4. Draw The Tile Map
 * 5. Draw The enemies and obstacles
 * 6. Draw The Player
 * 7. Display everything on screen
 */
static void draw_scene(const Player* player, const Rectangle objects[],
                       int object_count, const Enemy* enemy,
                       const Apple apples[], int apple_count,
                       int collected_apple_count, bool all_apples_collected)
{
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        /* // Display frame reference
        DrawTexture(player->texture, 15, 40, WHITE);
        DrawRectangleLines(15, 40, player->texture.width,
                           player->texture.height, LIME);
        DrawRectangleLines(15 + (int)player->frame_rectangle.x,
                           40 + (int)player->frame_rectangle.y,
                           (int)player->frame_rectangle.width,
                           (int)player->frame_rectangle.height, RED);
        // */
        for (int i = 0; i < object_count; ++i)
            DrawRectangleRec(objects[i], GRAY);
        draw_apples(apples, apple_count);
        draw_apple_progress(apple_count, collected_apple_count);
        draw_player(player);
        DrawRectangleRec(enemy->hurtbox, enemy->color);

        if (all_apples_collected)
        {
            DrawText("YOU WIN!", 900, 50, 50, GRAY);
            DrawText("THANKS FOR PLAYING", 830, 130, 30, GRAY);
        }
    }
    EndDrawing();
}

/**
 * @brief 
 * 
 * @param player 
 * @param enemy 
 */
static void handle_player_enemy_collision(Player* player, const Enemy* enemy)
{
    if (aabb_collision(enemy->hurtbox, get_player_hitbox(player)))
    {
        printf("Hits enemy.\n");
        player->status = Dead;
        player->respawn_countdown = FPS / 2;
    }
}
