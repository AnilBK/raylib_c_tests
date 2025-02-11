// gcc -o highlight_text.exe highlight_text.c -lraylib -lgdi32 -lwinmm && highlight_text.exe

#include "raylib.h"
#include <string.h>
#include <stdio.h>

#define FONT_SIZE 32
#define MAX_TEXT_LENGTH 128
#define DELETE_SPEED 10

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Raylib - Eraser Effect with Hold-to-Delete");

    Font font = LoadFontEx("Recursive-Regular.ttf", FONT_SIZE, NULL, 0);
    if (!font.texture.id) {
        CloseWindow();
        printf("Failed to load font!\n");
        return 1;
    }

    char text[MAX_TEXT_LENGTH] = "Twinkle Twinkle Little Star, How I wonder what you are !!!!";  
    Vector2 textPosition = { 20, 200 };
    int deleteCounter = 0;

    HideCursor();
    SetTargetFPS(60);

    while (!WindowShouldClose()) 
    {
        Vector2 mousePos = GetMousePosition();
        int length = strlen(text);
        int deleteIndex = -1;
        float charWidth = FONT_SIZE / 2.0f; // Default character width for eraser
        Vector2 eraserPos = { mousePos.x - charWidth / 2, mousePos.y - FONT_SIZE / 2 };

        for (int i = 0; i < length; i++)
        {
            float currentCharWidth = MeasureTextEx(font, TextFormat("%c", text[i]), FONT_SIZE, 2).x;
            Vector2 charPos = { textPosition.x, textPosition.y };
            
            for (int j = 0; j < i; j++)
                charPos.x += MeasureTextEx(font, TextFormat("%c", text[j]), FONT_SIZE, 2).x;

            if (mousePos.x >= charPos.x && mousePos.x <= charPos.x + currentCharWidth &&
                mousePos.y >= charPos.y && mousePos.y <= charPos.y + FONT_SIZE)
            {
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                {
                    if (deleteCounter % DELETE_SPEED == 0)
                    {
                        deleteIndex = i;
                    }
                    deleteCounter++;
                }
                else
                {
                    deleteCounter = 0;
                }
                
                eraserPos.x = charPos.x;
                eraserPos.y = charPos.y;
                charWidth = currentCharWidth;
            }
        }

        if (deleteIndex != -1)
        {
            #if 1
            for (int i = deleteIndex; i < length; i++)
            {
                text[i] = text[i + 1]; 
            }
            #else
                text[deleteIndex] = ' ';
            #endif
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        Vector2 drawPos = textPosition;
        for (int i = 0; i < strlen(text); i++)
        {
            float currentCharWidth = MeasureTextEx(font, TextFormat("%c", text[i]), FONT_SIZE, 2).x;
            Vector2 charPos = drawPos;

            DrawTextEx(font, TextFormat("%c", text[i]), charPos, FONT_SIZE, 2, BLACK);
            drawPos.x += currentCharWidth;
        }

        // Always draw the eraser box
        DrawRectangle(eraserPos.x, eraserPos.y, charWidth, FONT_SIZE, Fade(WHITE, 0.5f));
        DrawRectangleLines(eraserPos.x, eraserPos.y, charWidth, FONT_SIZE, BLACK);

        DrawText("Hover over a character to erase", 10, 10, 20, DARKGRAY);

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}