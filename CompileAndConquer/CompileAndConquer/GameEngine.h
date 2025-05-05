#pragma once
#include "Map.h"
#include <string>

class GameEngine {
public:
    GameEngine();
    void run();
    void displayTitleScreen();
    void startNewGame();
    std::string promptPlayerName();
    void clearScreen();
    char promptNewGameOption();

private:
    Map map;
    int playerX;
    int playerY;
    int level;
    std::string playerName;


    void render();
    void processInput();
};
