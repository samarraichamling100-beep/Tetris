#include "raylib.h"
#include <iostream>
#include <vector>
#include <algorithm>
class GRID
{
    private:
    typedef std::vector<std::vector<int>> Matrix;
    int screenwidth  = 800;
    int screenheight = 800;
    int Xoffset      = 200;
    int cellsize     = 40;
    int MoveCooldown = 20;
    int RotationCooldown = 0;
    unsigned int PullTickRate = 0;
    int         MovementCooldown = 0 ;
    bool        HasPulled{};
    int         FallMeter{};

    public:
    int grid[20][10] = {
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0}
    };


void Check_rows() {
    for (int r = 0; r < 20; r++) {
        int lockedCount = 0;
        
        for (int c = 0; c < 10; c++) {
            if (grid[r][c] == 2) {
                lockedCount++;
            }
        }
        
        if (lockedCount == 10) {
            for (int space = r; space > 0; space--) {
                for (int col = 0; col < 10; col++) {
                    grid[space][col] = grid[space - 1][col];
                }
            }
                for (int col = 0; col < 10; col++) {
                    grid[0][col] = 0;
                }
                r--; 
            }
        }
    }

    void DrawTetrisGrid()
    {
        for(unsigned int col = 0; col < 21; col++)
        {
            DrawLine(Xoffset, col * cellsize, Xoffset + 400, col * cellsize, BLACK);
        }
        for(unsigned int row = 0; row < 11; row++)
        {
            DrawLine(Xoffset + row * cellsize, 0, Xoffset + row * cellsize, screenheight, BLACK);
        }
    }

    bool IsValidPosition(int x, int y, const int tempGrid[20][10])
    {
        if (x < 0 || x >= 10 || y < 0 || y >= 20) return false;
        if (tempGrid[y][x] == 2) return false;
        return true;
    }

    bool TryRotationWithWallKick(auto (&CurrentPiece)[4], auto &CenterPiece)
    {
        int cleanGrid[20][10];
        for (int col = 0; col < 20; col++) {
            for (int row = 0; row < 10; row++) {
                cleanGrid[col][row] = (grid[col][row] == 1) ? 0 : grid[col][row];
            }
        }

        struct Point { int x, y; };
        Point rotatedPositions[4];
        for (int i = 0; i < 4; i++) {
            int relX = CurrentPiece[i].x - CenterPiece.x;
            int relY = CurrentPiece[i].y - CenterPiece.y;
            rotatedPositions[i].x = CenterPiece.x - relY;
            rotatedPositions[i].y = CenterPiece.y + relX;
        }

        Point kickOffsets[3] = { {0, 0}, {-1, 0}, {1, 0} };

        for (int k = 0; k < 3; k++) {
            bool testPassed = true;
            for (int i = 0; i < 4; i++) {
                int testX = rotatedPositions[i].x + kickOffsets[k].x;
                int testY = rotatedPositions[i].y + kickOffsets[k].y;

                if (!IsValidPosition(testX, testY, cleanGrid)) {
                    testPassed = false;
                    break;
                }
            }
            if (testPassed) {
                for (int i = 0; i < 4; i++) {
                    if (CurrentPiece[i].y >= 0 && CurrentPiece[i].y < 20 && CurrentPiece[i].x >= 0 && CurrentPiece[i].x < 10) {
                        grid[CurrentPiece[i].y][CurrentPiece[i].x] = 0;
                    }
                }
                CenterPiece.x += kickOffsets[k].x;
                CenterPiece.y += kickOffsets[k].y;
                for (int i = 0; i < 4; i++) {
                    CurrentPiece[i].x = rotatedPositions[i].x + kickOffsets[k].x;
                    CurrentPiece[i].y = rotatedPositions[i].y + kickOffsets[k].y;
                    grid[CurrentPiece[i].y][CurrentPiece[i].x] = 1;
                }
                return true; 
            }
        }
        return false;
    }

    void LoadPiece(auto (&CurrentPiece)[4], auto &CenterPiece)
    {
        MovementCooldown++;
        PullTickRate++;
        RotationCooldown++;
        int MoveX = 0;
        if(IsKeyPressed(KEY_A) && MovementCooldown > 15){
            MoveX = -1;
            MovementCooldown = 0;
        }
        if(IsKeyPressed(KEY_D) && MovementCooldown > 15){
            MoveX = 1;
            MovementCooldown = 0;
        }
        
        // Updated rotation input execution utilizing wall kick checks
        if(IsKeyPressed(KEY_R) && RotationCooldown > 15)
        {
            TryRotationWithWallKick(CurrentPiece, CenterPiece);
            RotationCooldown = 0;
        }
        
        int CheckGrid[20][10];
        if(MoveX != 0) {
            for (int col = 0; col < 20; col++)
            {
                for(int row = 0; row < 10; row++)
                {
                    if(grid[col][row] != 1) CheckGrid[col][row] = grid[col][row];
                    else CheckGrid[col][row] = 0;
                }
            }
        }
        
        bool validMove = true;
        for (int col = 0; col < 20; col++) {
            for (int row = 0; row < 10; row++) {
                if (grid[col][row] == 1) {
                    int nextRow = row + MoveX;
                    if (nextRow >= 0 && nextRow < 10 && grid[col][nextRow] != 2) {
                        CheckGrid[col][nextRow] = 1;
                    } else {
                        validMove = false;
                        MoveX = 0; 
                        break;
                    }
                }
            }
            if (!validMove) break;
        }

        if (MoveX != 0) {
            for(unsigned index = 0; index < 4; index++)
            {
                CurrentPiece[index].x += MoveX;
            }
            for (int c = 0; c < 20; c++) {
                for (int r = 0; r < 10; r++) {
                    grid[c][r] = CheckGrid[c][r];
                }
            }
        }
    
        if (PullTickRate >= 15) {
            PullTickRate = 0; 
            bool CanFall = true;
            for (int col = 19; col >= 0; col--) {
                for (int row = 0; row < 10; row++) {
                    if (grid[col][row] == 1) {
                        if (col == 19 || grid[col + 1][row] == 2)
                        {
                            CanFall = false;
                        }
                    }
                }
            }
            
            if (!CanFall)
            {
                for (int col = 0; col < 20; col++)
                {
                    for (int row = 0; row < 10; row++)
                    {
                        if (grid[col][row] == 1)
                        {
                            grid[col][row] = 2;
                        }
                    }
                }
            }
            else {
                for (int col = 19; col >= 0; col--)
                {
                    for (int row = 0; row < 10; row++)
                    {
                        if (grid[col][row] == 1)
                        {
                            grid[col][row] = 0;
                            grid[col + 1][row] = 1;
                            FallMeter++;
                        }
                        else if(FallMeter == 4)
                        {
                            for(unsigned int index = 0; index < 4; index++)
                            {
                                CurrentPiece[index].y += 1;
                            }
                            FallMeter = 0;
                        }
                    }
                }
            }
        }
        
        for(unsigned int col = 0; col < 20; col++)
        {
            for(unsigned int row = 0; row < 10; row++)
            {
                if(grid[col][row] == 1)
                {
                    Rectangle Block = {float(Xoffset + row * cellsize), float(col * cellsize), (float)cellsize, (float)cellsize};
                    DrawRectangleRec(Block, RED);
                }
                if(grid[col][row] == 2)
                {
                    Rectangle Block = {float(Xoffset + row * cellsize), float(col * cellsize), (float)cellsize, (float)cellsize};
                    DrawRectangleRec(Block, GREEN);
                }
            }
        }
    }
};
