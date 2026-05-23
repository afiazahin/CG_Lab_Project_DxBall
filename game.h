#ifndef GAME_H
#define GAME_H

// -------------------- Game configuration --------------------
const int WINDOW_WIDTH  = 800;
const int WINDOW_HEIGHT = 600;

const int ROWS = 6;
const int COLS = 10;

// Different pages/states of the game.
enum GameState
{
    MENU,
    PLAYING,
    PAUSED,
    HELP,
    GAMEOVER,
    WIN
};

// Perk type dropped from bricks.
enum PerkType
{
    PERK_NONE,
    PERK_EXTRA_LIFE,
    PERK_FAST_BALL,
    PERK_WIDE_PADDLE
};

struct Perk
{
    float x, y;
    float size;
    float speed;
    bool active;
    PerkType type;
};

// -------------------- Global game variables --------------------
extern GameState currentState;

extern float paddleX;
extern float paddleY;
extern float paddleWidth;
extern float paddleHeight;

extern float ballX;
extern float ballY;
extern float ballRadius;
extern float ballDX;
extern float ballDY;
extern bool ballLaunched;

extern int score;
extern int lives;
extern int level;
extern int elapsedTime;
extern int highScore;

extern bool bricks[ROWS][COLS];
extern int brickColor[ROWS][COLS];
extern Perk perks[ROWS * COLS];

// -------------------- Game functions --------------------
void initializeGame();
void resetGame();
void updateGame();
void updateTimer();
void loadHighScore();
void saveHighScore();
void startNextLevel();

#endif
