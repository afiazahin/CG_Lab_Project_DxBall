#ifndef GAME_H
#define GAME_H

enum GameState
{
    MENU,
    PLAYING,
    PAUSED,
    HELP,
    GAMEOVER,
    WIN
};

extern GameState currentState;

extern float paddleX;
extern float paddleWidth;

void initializeGame();
void updateGame();
void resetGame();

#endif