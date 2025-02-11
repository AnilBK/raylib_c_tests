#include "raylib.h"
#include "time.h"
#include "stdlib.h"

// gcc -o drawer.exe drawer.c -lraylib -lgdi32 -lwinmm && drawer.exe

const Vector2 DRAWER_SIZE = {685, 500};
const Vector2 DRAWER_HEAD_SIZE = {175, 40};

enum HEAD_TYPE {
    LEFT,
    MIDDLE,
    RIGHT
};

struct drawer {
    Vector2 position;
    bool is_being_dragged;
    enum HEAD_TYPE head_type;
};


Rectangle get_head_rect(struct drawer *drawer) {
    if (drawer->head_type == LEFT) {
        return (Rectangle){drawer->position.x, drawer->position.y, DRAWER_HEAD_SIZE.x, DRAWER_HEAD_SIZE.y};
    } else if (drawer->head_type == MIDDLE) {
        return (Rectangle){drawer->position.x + 245, drawer->position.y, DRAWER_HEAD_SIZE.x, DRAWER_HEAD_SIZE.y};
    } else if (drawer->head_type == RIGHT) {
        return (Rectangle){drawer->position.x + 510, drawer->position.y, DRAWER_HEAD_SIZE.x, DRAWER_HEAD_SIZE.y};
    }
    return (Rectangle){0, 0, 0, 0};
}

void draw_drawer(struct drawer *drawer) {
    Rectangle head_rect = get_head_rect(drawer);
    DrawRectangleRec(head_rect, WHITE);
    DrawRectangleLinesEx(head_rect, 2, BLACK);
    DrawText("Drawer", head_rect.x + 10, head_rect.y + 10, 20, BLACK);

    Rectangle drawer_body_rect = {drawer->position.x, drawer->position.y + DRAWER_HEAD_SIZE.y, DRAWER_SIZE.x, DRAWER_SIZE.y};
    DrawRectangleRec(drawer_body_rect, WHITE);
    DrawRectangleLinesEx(drawer_body_rect, 2, BLACK);

    DrawCircle(drawer->position.x + DRAWER_SIZE.x / 2,  drawer->position.y + DRAWER_SIZE.y / 2, 145, RED);

}

void handle_input(struct drawer *drawer){
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse_position = GetMousePosition();

        if (CheckCollisionPointRec(mouse_position, get_head_rect(drawer))) {
            drawer->is_being_dragged = true;
        }
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        drawer->is_being_dragged = false;
    }

    if (drawer->is_being_dragged) {
        drawer->position.y = GetMousePosition().y;
    }
}

int main(void)
{

    srand(time(NULL));

    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - basic shapes drawing");

    SetTargetFPS(60);

    #define TOTAL_DRAWERS 7

    struct drawer drawers[TOTAL_DRAWERS] = {0};
    for (int i = 0; i < TOTAL_DRAWERS; i++) {
        drawers[i].position = (Vector2){ 45, 45 + (i * 60)};
        drawers[i].is_being_dragged = false;
        int rnd = (i + 1) % 3;//rand() % 3;
        if (rnd == 0){
            drawers[i].head_type = LEFT;
        } else if (rnd == 1) {
            drawers[i].head_type = MIDDLE;
        } else {
            drawers[i].head_type = RIGHT;
        }
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);

            for (int i = 0; i < TOTAL_DRAWERS; i++) {
                draw_drawer(&drawers[i]);
            }

        EndDrawing();

        for (int i = 0; i < TOTAL_DRAWERS; i++) {
            handle_input(&drawers[i]);
        }
    }

    CloseWindow();

    return 0;
}