#pragma once
#include "Map.h"

class GameEngine {
public:
    GameEngine();
    void run();

private:
    Map map;
    int playerX;
    int playerY;

    void render();
    void processInput();
};
