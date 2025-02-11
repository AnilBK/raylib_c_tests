// gcc -o ImageViewer.exe ImageViewer.c -lraylib -lgdi32 -lwinmm && ImageViewer.exe

#include "raylib.h"

#define SCREEN_WIDTH GetScreenWidth()
#define SCREEN_HEIGHT GetScreenHeight()

struct ImageViewer{
    Texture2D imageTexture;
    Vector2 imagePos;
    Rectangle closeButtonRec;
    bool dragging;
    Vector2 dragOffset;
};

void ImageViewerInit(struct ImageViewer *viewer, const char *imagePath) {
    viewer->imageTexture = LoadTexture(imagePath);
    viewer->imagePos = (Vector2){ (SCREEN_WIDTH - viewer->imageTexture.width) / 2, 
                                  (SCREEN_HEIGHT - viewer->imageTexture.height) / 2 };
    viewer->closeButtonRec = (Rectangle){ viewer->imagePos.x + viewer->imageTexture.width - 20, viewer->imagePos.y, 20, 20 };
    viewer->dragging = false;
    viewer->dragOffset = (Vector2){0, 0};
}

void ImageViewerFree(struct ImageViewer *viewer) {
    UnloadTexture(viewer->imageTexture);
}

void ImageViewerUpdate(struct ImageViewer *viewer) {
    Vector2 mouse = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, (Rectangle){ viewer->imagePos.x, viewer->imagePos.y, viewer->imageTexture.width, viewer->imageTexture.height })) {
            viewer->dragging = true;
            viewer->dragOffset.x = mouse.x - viewer->imagePos.x;
            viewer->dragOffset.y = mouse.y - viewer->imagePos.y;
        } else if (CheckCollisionPointRec(mouse, viewer->closeButtonRec)) {
            // TODO
        }
    }
    
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        viewer->dragging = false;
    }

    if (viewer->dragging) {
        viewer->imagePos.x = mouse.x - viewer->dragOffset.x;
        viewer->imagePos.y = mouse.y - viewer->dragOffset.y;
        viewer->closeButtonRec.x = viewer->imagePos.x + viewer->imageTexture.width - 20;
        viewer->closeButtonRec.y = viewer->imagePos.y;
    }
}

void ImageViewerRender(struct ImageViewer *viewer) {
    DrawTextureEx(viewer->imageTexture, viewer->imagePos, 0.0f, 1.0f, WHITE);
    DrawLineEx((Vector2){viewer->closeButtonRec.x, viewer->closeButtonRec.y}, (Vector2){viewer->closeButtonRec.x + viewer->closeButtonRec.width, viewer->closeButtonRec.y + viewer->closeButtonRec.height}, 2, RED);
    DrawLineEx((Vector2){viewer->closeButtonRec.x + viewer->closeButtonRec.width, viewer->closeButtonRec.y}, (Vector2){viewer->closeButtonRec.x, viewer->closeButtonRec.y + viewer->closeButtonRec.height}, 2, RED);
}


int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Draggable Image with Close Button");
    ToggleFullscreen();

    struct ImageViewer viewer;
    ImageViewerInit(&viewer, "my_amazing_texture_painting.png");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        ImageViewerUpdate(&viewer);

        BeginDrawing();
        ClearBackground((Color){ 231, 229, 228, 255 });

        ImageViewerRender(&viewer);

        EndDrawing();
    }

    ImageViewerFree(&viewer);
    CloseWindow();

    return 0;
}
