// gcc -o ImageViewer.exe ImageViewer.c -lraylib -lgdi32 -lwinmm && ImageViewer.exe

#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Draggable Image with Close Button");

    Texture2D imageTexture = LoadTexture("my_amazing_texture_painting.png");
    Vector2 imagePos = { (SCREEN_WIDTH - imageTexture.width) / 2, 
                         (SCREEN_HEIGHT - imageTexture.height) / 2 };
    
    Rectangle closeButton = { imagePos.x + imageTexture.width - 20, imagePos.y, 20, 20 };
    bool dragging = false;
    Vector2 dragOffset = {0, 0};

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, (Rectangle){ imagePos.x, imagePos.y, imageTexture.width, imageTexture.height })) {
                dragging = true;
                dragOffset.x = mouse.x - imagePos.x;
                dragOffset.y = mouse.y - imagePos.y;
            } else if (CheckCollisionPointRec(mouse, closeButton)) {
                // TODO
            }
        }
        
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            dragging = false;
        }

        if (dragging) {
            imagePos.x = mouse.x - dragOffset.x;
            imagePos.y = mouse.y - dragOffset.y;
            closeButton.x = imagePos.x + imageTexture.width - 20;
            closeButton.y = imagePos.y;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawCircle(200, 200, 100, SKYBLUE);

        DrawTexture(imageTexture, imagePos.x, imagePos.y, WHITE);
        DrawRectangleRec(closeButton, RED);
        DrawLine(closeButton.x + 4, closeButton.y + 4, closeButton.x + 16, closeButton.y + 16, WHITE);
        DrawLine(closeButton.x + 16, closeButton.y + 4, closeButton.x + 4, closeButton.y + 16, WHITE);

        EndDrawing();
    }

    UnloadTexture(imageTexture);
    CloseWindow();

    return 0;
}
