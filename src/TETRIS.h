#include <iostream>
#include <raylib.h>
#include <random>

struct Vector2Int {
    int x;
    int y;
};
class TETRIS
{
    private:
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<int>piecetypeGen{0,4};
    std::uniform_int_distribution<int>colorindex{0,4};
    int pieceType;
    bool CanSpawnPiece = true;

    
    public:
    Vector2Int currentPiece[4];
    Vector2Int centerPoint;
    void SpawnPiece(int (* grid)[20][10])
    {

        pieceType = piecetypeGen(gen);
        switch (pieceType) {
            case 0:
            currentPiece[0] = {5,0};
            currentPiece[1] = {5,1};
            currentPiece[2] = {5,2};
            currentPiece[3] = {6,2};
            centerPoint = currentPiece[1];
                break;
            case 1:
            currentPiece[0] = {5,0};
            currentPiece[1] = {5,1};
            currentPiece[2] = {5,2};
            currentPiece[3] = {5,3};
            centerPoint = currentPiece[1];
                break;
            case 2:
            currentPiece[0] = {5,0};
            currentPiece[1] = {5,1};
            currentPiece[2] = {6,0};
            currentPiece[3] = {6,1};
            centerPoint = currentPiece[0];
                break;
            case 3: 
            currentPiece[0] = {5,0};
            currentPiece[1] = {5,1};
            currentPiece[2] = {6,1};
            currentPiece[3] = {4,1};
            centerPoint = currentPiece[1];
                break;
            case 4:
            currentPiece[0] = {5,1};
            currentPiece[1] = {6,1};
            currentPiece[2] = {6,0};
            currentPiece[3] = {7,0};
            centerPoint = currentPiece[1];
                break;
        }
        for(unsigned int i = 0;i < 4;i++)
        {
            (*grid)[currentPiece[i].y][currentPiece[i].x] = 1;
        }
    }
    void ShouldSpawnPiece(int (*grid)[20][10])
    {
        centerPoint = currentPiece[1];
        bool activePieceFound = false;

        for (int c = 0; c < 20; c++)
        {
            for (int r = 0; r < 10; r++)
            {
                if ((*grid)[c][r] == 1) {
                    activePieceFound = true; 
                    break;
                }
            }
        if (activePieceFound) break;
        }

        if (!activePieceFound) {
            SpawnPiece(grid);
        }
    }
};