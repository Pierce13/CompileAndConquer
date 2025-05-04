#include "GameEngine.h"
#include <iostream>

GameEngine::GameEngine() {
    Position start = map.generateLevel();  // Get the player start position from the map
    playerX = start.x;
    playerY = start.y;
}

void GameEngine::run() {
    while (true) {
        render();
        processInput();
    }
}

void GameEngine::render() {
    system("cls"); // Windows
    //system("clear"); // Mac
    map.render(playerX, playerY);
}

void GameEngine::processInput() {
    char input;
    std::cin >> input;

    int newX = playerX;
    int newY = playerY;

    switch (input) {
        case 'w': newY--; break;
        case 's': newY++; break;
        case 'a': newX--; break;
        case 'd': newX++; break;
        case 'q': exit(0); // Quit
    }

    if (map.isWalkable(newX, newY)) {
        playerX = newX;
        playerY = newY;
    }
}
