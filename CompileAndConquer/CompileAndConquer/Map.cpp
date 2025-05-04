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

void Map::render(int playerX, int playerY) const {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (x == playerX && y == playerY)
                std::cout << tileToChar(TileType::Player);
            else
                std::cout << tileToChar(grid[y][x]);
        }
        std::cout << '\n';
    }
}

bool Map::isWalkable(int x, int y) const {
    return grid[y][x] == TileType::Floor;
}

Position Map::generateLevel() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Fill with walls
    for (int y = 0; y < HEIGHT; ++y)
        for (int x = 0; x < WIDTH; ++x)
            grid[y][x] = TileType::Wall;

    const int maxRooms = 30;                // More rooms
    const int roomMinSize = 3;              // Smaller min size
    const int roomMaxSize = 7;              // Smaller max size
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

        // Carve room
        for (int ry = y; ry < y + h; ++ry)
            for (int rx = x; rx < x + w; ++rx)
                grid[ry][rx] = TileType::Floor;

        // Connect to previous
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

    // Ensure the player spawns in a valid floor tile
    Position playerPosition{ -1, -1 };
    for (const Room& room : rooms) {
        int centerX = room.centerX();
        int centerY = room.centerY();

        if (grid[centerY][centerX] == TileType::Floor) {
            playerPosition = { centerX, centerY };
            break;
        }
    }

    // Place stairs at the farthest point
    if (!rooms.empty()) {
        int sx = rooms.back().centerX();
        int sy = rooms.back().centerY();
        grid[sy][sx] = TileType::Stairs;
    }

    // If no valid position found, use a fallback
    if (playerPosition.x == -1 || playerPosition.y == -1) {
        playerPosition = { 1, 1 };  // Fallback to top-left corner if something goes wrong
    }

    return playerPosition;
}

void Map::resetLevel() {
    // Reset the grid to walls and regenerate level
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            grid[y][x] = TileType::Wall;
        }
    }
    generateLevel(); // Regenerate the level with new rooms
}

