#include "raylib.h"
#include "stddef.h"

// gcc -o text_editor.exe text_editor.c -lraylib -lgdi32 -lwinmm && text_editor.exe

typedef struct {
    const char *text;
    Vector2 position;
    float fontSize;
    float spacing;
    Color color;
    bool dragging;
    Vector2 offset;
} DraggableText;

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Load TTF Font in Raylib");
    
    Font font = LoadFontEx("Recursive-Regular.ttf", 32, NULL, 0);
    
    DraggableText text1 = {"Hello, Raylib!", {100, 200}, 32, 2, BLACK, false, {0, 0}};
    DraggableText text2 = {"This is a small paragraph.", {100, 300}, 24, 2, BLACK, false, {0, 0}};
    
    SetTargetFPS(60);
    
    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Rectangle rect1 = {text1.position.x, text1.position.y, MeasureTextEx(font, text1.text, text1.fontSize, text1.spacing).x, text1.fontSize};
            Rectangle rect2 = {text2.position.x, text2.position.y, MeasureTextEx(font, text2.text, text2.fontSize, text2.spacing).x, text2.fontSize};
            
            if (CheckCollisionPointRec(mousePos, rect1)) {
                text1.dragging = true;
                text1.offset = (Vector2){mousePos.x - text1.position.x, mousePos.y - text1.position.y};
            } else if (CheckCollisionPointRec(mousePos, rect2)) {
                text2.dragging = true;
                text2.offset = (Vector2){mousePos.x - text2.position.x, mousePos.y - text2.position.y};
            }
        }
        
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            text1.dragging = false;
            text2.dragging = false;
        }
        
        if (text1.dragging) text1.position = (Vector2){mousePos.x - text1.offset.x, mousePos.y - text1.offset.y};
        if (text2.dragging) text2.position = (Vector2){mousePos.x - text2.offset.x, mousePos.y - text2.offset.y};
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        Rectangle rect1 = {text1.position.x, text1.position.y, MeasureTextEx(font, text1.text, text1.fontSize, text1.spacing).x, text1.fontSize};
        Rectangle rect2 = {text2.position.x, text2.position.y, MeasureTextEx(font, text2.text, text2.fontSize, text2.spacing).x, text2.fontSize};
        
        DrawRectangleLines(rect1.x, rect1.y, rect1.width, rect1.height, BLUE);
        DrawRectangleLines(rect2.x, rect2.y, rect2.width, rect2.height, BLUE);
        
        DrawTextEx(font, text1.text, text1.position, text1.fontSize, text1.spacing, text1.color);
        DrawTextEx(font, text2.text, text2.position, text2.fontSize, text2.spacing, text2.color);
        
        EndDrawing();
    }
    
    UnloadFont(font);
    CloseWindow();
    
    return 0;
}
