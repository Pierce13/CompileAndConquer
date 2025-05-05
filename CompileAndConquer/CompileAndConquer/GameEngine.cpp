#include "GameEngine.h"
#include <iostream>
#include <limits>  // For std::numeric_limits

GameEngine::GameEngine() {
    displayTitleScreen();
    char choice = promptNewGameOption();
    if (choice == 'N' || choice == 'n') {
        // Clear the screen and start the new game
        clearScreen();
        startNewGame();
    }
}

void GameEngine::displayTitleScreen() {
    std::string title = "_________                       .__.__                              .___ _________                                           \n"
        "\\_   ___ \\  ____   _____ ______ |__|  |   ____   _____    ____    __| _/ \\_   ___ \\  ____   ____   ________ __   ___________ \n"
        "/    \\  \\/ /  _ \\ /     \\\\____ \\|  |  | _/ __ \\  \\__  \\  /    \\  / __ |  /    \\  \\/ /  _ \\ /    \\ / ____/  |  \\_/ __ \\_  __ \\ \n"
        "\\     \\___(  <_> )  Y Y  \\  |_> >  |  |_\\  ___/   / __ \\|   |  \\/ /_/ |  \\     \\___(  <_> )   |  < <_|  |  |  /\\  ___/|  | \\/ \n"
        " \\______  /\\____/|__|_|  /   __/|__|____/\\___  > (____  /___|  /\\____ |   \\______  /\\____/|___|  /\\__   |____/  \\___  >__|   \n"
        "        \\/             \\/|__|                \\/       \\/     \\/      \\/          \\/            \\/    |__|           \\/        \n";

    std::cout << title;
    std::cout << "\n(N)ew Game.";
}

char GameEngine::promptNewGameOption() {
    char choice;
    do {
        std::cin >> choice;
        // Flush the input buffer after reading the choice to clear any leftover newline
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (tolower(choice) != 'n');

    return choice;
}

void GameEngine::clearScreen() {
    system("cls");  // For Windows
    // system("clear"); // For Unix-based systems, use "clear"
}

void GameEngine::startNewGame() {
    playerName = promptPlayerName();
    playerHP = 50;
    playerMaxHP = 50;
    std::cout << "Game starting...\n";
    Position start = map.generateLevel();
    playerX = start.x;
    playerY = start.y;
    level = 1;
}

std::string GameEngine::promptPlayerName() {
    std::string name;

    std::cout << "Enter your character's name: ";
    std::getline(std::cin, name);

    return name;
}

void GameEngine::run() {
    while (true) {
        render();
        processInput();
    }
}

void GameEngine::render() {
    system("cls"); // Windows: clear the screen
    // system("clear"); // Mac/Linux: clear the screen
    map.render(playerX, playerY, playerName, level, playerHP, playerMaxHP);
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

        // Check if the player is at the stairs
        if (playerX == map.stairsX && playerY == map.stairsY) {
            Position newStart = map.generateLevel();
            playerX = newStart.x;
            playerY = newStart.y;
            level++;
        }
    }
}
