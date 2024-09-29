#include "player/inc/player.h"
#include "collision/inc/collision.h"
#include "enemy/inc/enemy.h"
#include "apple/inc/apple.h"
#include "raylib.h"
#include <stdio.h> // printf()

#define FPS 60

static void draw_scene(const Player* player, const Rectangle objects[],
                       int object_count, const Enemy enemy[],
                       const Apple apples[], int apple_count);
static void update_player(Player* player, const Rectangle objects[],
                          int object_count, const Enemy* enemy);

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

    Rectangle floor = {
        .x = 0, .y = screen_height - 200, .width = screen_width, .height = 200};
    Rectangle left_block = {
        .x = 200, .y = screen_height - 400, .width = 250, .height = 200};
    Rectangle block_in_air = {.x = 700, .y = 300, .width = 200, .height = 100};
    Rectangle right_block = {
        .x = 1200, .y = screen_height - 400, .width = 250, .height = 200};

    Rectangle objects[] = {floor, left_block, right_block, block_in_air};
    Enemy enemy = build_enemy((Rectangle){.x = left_block.x + left_block.width,
                                          .y = floor.y - 100.0f,
                                          .height = 100.0f,
                                          .width = 100.0f},
                              left_block.x + left_block.width - 1,
                              right_block.x - 100 + 1, RED, 2.0f);

    Apple apples[] = {
        (Apple){NotCollected,
                (Rectangle){.x = 775, .y = 250, .height = 50, .width = 50}},
        (Apple){NotCollected,
                (Rectangle){
                    .x = 800, .y = floor.y - 50, .height = 50, .width = 50}},
        (Apple){NotCollected,
                (Rectangle){
                    .x = 1700, .y = floor.y - 50, .height = 50, .width = 50}}};
    int  apple_count = sizeof(apples) / sizeof(apples[0]);
    bool all_apples_collected = false;

    SetTargetFPS(FPS);
    //--------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        if (player.respawn_countdown > 0)
        {
            draw_scene(&player, objects, sizeof(objects) / sizeof(objects[0]),
                       &enemy, apples, apple_count);
            --(player.respawn_countdown);
            continue;
        }
        if (player.status == Dead)
            respawn_player(&player, 100, 600);

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

        update_player(&player, objects, sizeof(objects) / sizeof(objects[0]),
                      &enemy);
        {
            Rectangle hitbox = get_player_hitbox(&player);
            for (int i = 0; i < apple_count; ++i)
            {
                if (apples[i].status == Collected)
                    continue;
                if (aabb_collision(apples[i].box, hitbox))
                    apples[i].status = Collected;
            }
        }
        //----------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------
        draw_scene(&player, objects, sizeof(objects) / sizeof(objects[0]),
                   &enemy, apples, apple_count);
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
                       int object_count, const Enemy enemy[],
                       const Apple apples[], int apple_count)
{
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        DrawTexture(player->texture, 15, 40, WHITE);
        DrawRectangleLines(15, 40, player->texture.width,
                           player->texture.height, LIME);
        DrawRectangleLines(15 + (int)player->frame_rectangle.x,
                           40 + (int)player->frame_rectangle.y,
                           (int)player->frame_rectangle.width,
                           (int)player->frame_rectangle.height, RED);
        for (int i = 0; i < object_count; ++i)
            DrawRectangleRec(objects[i], GRAY);
        for (int i = 0; i < apple_count; ++i)
            if (apples[i].status == NotCollected)
                DrawRectangleRec(apples[i].box, GREEN);
        draw_player(player);
        DrawRectangleRec(enemy->hurtbox, enemy->color);
    }
    EndDrawing();
}

/**
 * @brief Updates player position and solves collision. Updates texture frame.
 *        Checks enemy hit.
 *
 * @param player Pointer to player struct
 * @param objects Pointer to objects array
 * @param object_count Number of objects in array
 * @param enemy Pointer to enemy struct
 * @note
 * Method: Interleaving single-axis movement and collision detection
 */
static void update_player(Player* player, const Rectangle objects[],
                          int object_count, const Enemy* enemy)
{
    // Update x-axis and solve collision
    player->position.x += player->velocity.x;
    for (int i = 0; i < object_count; ++i)
    {
        Rectangle hitbox = get_player_hitbox(player);
        if (aabb_collision(objects[i], hitbox))
        {
            if (player->velocity.x > 0) // going right
            {
                player->position.x = objects[i].x - hitbox.width / 2 - 0.1f;
            }
            else if (player->velocity.x < 0) // going left
            {
                player->position.x = objects[i].x + objects[i].width
                                     + hitbox.width / 2 + 0.1f;
            }
            player->velocity.x = 0;
        }
    }

    // Update y-axis and solve collision
    player->position.y += player->velocity.y;
    for (int i = 0; i < object_count; ++i)
    {
        if (aabb_collision(objects[i], get_player_hitbox(player)))
        {
            if (player->velocity.y > 0)
            {
                // player lands on object
                player->position.y = objects[i].y;
                player->is_in_air = false;
                player->jumps_left = player->max_jumps;
                // printf("Jump reset\n");
            }
            else if (player->velocity.y < 0)
            {
                player->position.y = objects[i].y + objects[i].height
                                     + get_player_hitbox(player).height;
            }
            player->velocity.y = 0;
        }
    }

    if (player->frame_counter >= FPS / player->frame_rate)
    {
        player->frame_counter = 0;
        ++(player->frame_index);
        if (player->frame_index > 5)
            player->frame_index = 0;
        player->frame_rectangle.x = (float)player->frame_index
                                    * (float)player->texture.width / 6;
    }

    if (aabb_collision(enemy->hurtbox, get_player_hitbox(player)))
    {
        printf("Hits enemy.\n");
        player->status = Dead;
        player->respawn_countdown = FPS / 2;
    }
}
