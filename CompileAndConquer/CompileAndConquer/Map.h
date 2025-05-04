#pragma once
#include "Tile.h"
#include <vector>

struct Position {
    int x, y;
    Position(int x, int y) : x(x), y(y) {}
};

class Map {
public:
    static const int WIDTH = 50;
    static const int HEIGHT = 25;
    int stairsX = -1;
    int stairsY = -1;

    Map();
    void render(int playerX, int playerY) const;
    bool isWalkable(int x, int y) const;
    Position generateLevel();
    void resetLevel();

private:
    TileType grid[HEIGHT][WIDTH];
};

