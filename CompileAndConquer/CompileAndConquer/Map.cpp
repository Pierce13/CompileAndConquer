#include "Map.h"
#include <iostream>
#include <stack>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>

struct Room {
    int x, y, width, height;

    bool intersects(const Room& other) const {
        return !(x + width < other.x || other.x + other.width < x ||
            y + height < other.y || other.y + other.height < y);
    }

    int centerX() const { return x + width / 2; }
    int centerY() const { return y + height / 2; }
};

Map::Map() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    generateLevel();
}

void Map::render(int playerX, int playerY, const std::string& playerName, int levelNumber, int playerHP, int playerMaxHP) {

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (x == playerX && y == playerY)
                std::cout << tileToChar(TileType::Player);
            else if (x == stairsX && y == stairsY) {
                double distance = std::sqrt(std::pow(playerX - stairsX, 2) + std::pow(playerY - stairsY, 2));
                if (!stairsRevealed) {
                    if (distance <= 4) {
                        std::cout << tileToChar(TileType::Stairs);
                    }
                }
                else {
                    std::cout << tileToChar(TileType::Stairs);
                    stairsRevealed = true;
                }
            }
            else
                std::cout << tileToChar(grid[y][x]);
        }

        if (y == 0)
            std::cout << "   Name: " << playerName << " | HP: " << playerHP << "/" << playerMaxHP;
        else if (y == 1)
            std::cout << "   Level: " << levelNumber;

        std::cout << '\n';
    }
}

bool Map::isWalkable(int x, int y) const {
    if (grid[y][x] == TileType::Floor || grid[y][x] == TileType::Stairs) {
        return true;
    }
    else {
        return false;
    }
}

Position Map::generateLevel() {
    stairsRevealed = false;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int y = 0; y < HEIGHT; ++y)
        for (int x = 0; x < WIDTH; ++x)
            grid[y][x] = TileType::Wall;

    const int maxRooms = 30;
    const int roomMinSize = 3; 
    const int roomMaxSize = 7;
    std::vector<Room> rooms;

    for (int i = 0; i < maxRooms; ++i) {
        int w = roomMinSize + std::rand() % (roomMaxSize - roomMinSize + 1);
        int h = roomMinSize + std::rand() % (roomMaxSize - roomMinSize + 1);
        int x = std::rand() % (WIDTH - w - 1) + 1;
        int y = std::rand() % (HEIGHT - h - 1) + 1;

        Room newRoom{ x, y, w, h };

        bool overlaps = false;
        for (const Room& r : rooms) {
            if (newRoom.intersects({ r.x - 1, r.y - 1, r.width + 2, r.height + 2 })) {
                overlaps = true;
                break;
            }
        }

        if (overlaps) continue;

        for (int ry = y; ry < y + h; ++ry)
            for (int rx = x; rx < x + w; ++rx)
                grid[ry][rx] = TileType::Floor;

        if (!rooms.empty()) {
            Room prev = rooms.back();
            int x1 = prev.centerX(), y1 = prev.centerY();
            int x2 = newRoom.centerX(), y2 = newRoom.centerY();

            if (std::rand() % 2 == 0) {
                for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x)
                    grid[y1][x] = TileType::Floor;
                for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y)
                    grid[y][x2] = TileType::Floor;
            }
            else {
                for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y)
                    grid[y][x1] = TileType::Floor;
                for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x)
                    grid[y2][x] = TileType::Floor;
            }
        }

        rooms.push_back(newRoom);
    }

    Position playerPosition{ -1, -1 };
    for (const Room& room : rooms) {
        int centerX = room.centerX();
        int centerY = room.centerY();

        if (grid[centerY][centerX] == TileType::Floor) {
            playerPosition = { centerX, centerY };
            break;
        }
    }

    if (!rooms.empty()) {
        stairsX = rooms.back().centerX();
        stairsY = rooms.back().centerY();
        grid[stairsY][stairsX] = TileType::Stairs;
    }

    if (playerPosition.x == -1 || playerPosition.y == -1) {
        playerPosition = { 1, 1 };
    }

    return playerPosition;
}



