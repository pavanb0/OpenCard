// #ifndef UI_MENUE_H
// #define UI_MENUE_H

// #include <Arduino.h>

// enum MenuState
// {
//     MENU_SPLASH,
//     MENU_HOME,
//     MENU_ABOUT,
//     MENU_GAME_SETUP,
//     MENU_GAME_PROGRESS
// };

// struct GameConfig
// {
//     int players;
//     int cardsPerPlayer;
// };

// void menuInit();
// void menuUpdate();
// void menuDraw();

// MenuState menuGetState();
// GameConfig menuGetConfig();

// #endif


#ifndef UI_MENUE_H
#define UI_MENUE_H

#include <Arduino.h>

enum MenuState
{
    MENU_SPLASH,
    MENU_HOME,
    MENU_ABOUT,
    MENU_GAME_SETUP,
    MENU_GAME_PROGRESS,
    MENU_GAME_DONE,
    MENU_GAME_JAM
};

struct GameConfig
{
    int players;
    int cardsPerPlayer;
};

// Live progress fed from gameControllerTask via menuSetProgress()
struct GameProgress
{
    int currentPlayer;
    int currentCard;
    int totalPlayers;
    int cardsPerPlayer;
    bool deckEmpty;
    bool jamDetected;
    bool finished;
};

void menuInit();
void menuUpdate();
void menuDraw();

MenuState menuGetState();
GameConfig menuGetConfig();

// Called from gameControllerTask (or a watcher task) to push progress
void menuSetProgress(const GameProgress &p);

#endif