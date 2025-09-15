
#include "raylib.h"

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [<module>] example - <name>");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawLineEx((Vector2){0, 0}, (Vector2){screenWidth, screenHeight}, 2.0f,
                   RED);
        DrawLineEx((Vector2){0, screenHeight}, (Vector2){screenWidth, 0}, 2.0f,
                   RED);
        DrawText("example base code template", 260, 400, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}