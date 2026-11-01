#include "GRID.h"
#include "TETRIS.h"
int main(void) {
    const int screenWidth  = 800;
    const int screenHeight = 840;
    const int fps          = 60;

    InitWindow(screenWidth, screenHeight, "Tetris");
    SetTargetFPS(fps);
    GRID grid;
    TETRIS tetris;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        grid.DrawTetrisGrid();
        grid.Check_rows();
        tetris.ShouldSpawnPiece(&grid.grid);
        grid.LoadPiece(tetris.currentPiece,tetris.centerPoint);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
