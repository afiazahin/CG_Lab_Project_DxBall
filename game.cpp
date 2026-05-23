#include "game.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

GameState currentState = MENU;

float paddleX = 350;
float paddleY = 40;
float paddleWidth = 110;
float paddleHeight = 16;

float ballX = 400;
float ballY = 300;
float ballRadius = 8;
float ballDX = 4;
float ballDY = 4;
bool ballLaunched = false;

int score = 0;
int lives = 3;
int level = 1;
int elapsedTime = 0;
int highScore = 0;

bool bricks[ROWS][COLS];
int brickColor[ROWS][COLS];
Perk perks[ROWS * COLS];

static int frameCounter = 0;
static float baseBallSpeed = 4.0f;
static const float BRICK_W = 70.0f;
static const float BRICK_H = 22.0f;
static const float BRICK_GAP = 5.0f;
static const float BRICK_START_X = 25.0f;
static const float BRICK_START_Y = 505.0f;

static float absoluteValue(float value)
{
    return value < 0 ? -value : value;
}

static void clampPaddle()
{
    if(paddleX < 0) paddleX = 0;
    if(paddleX + paddleWidth > WINDOW_WIDTH) paddleX = WINDOW_WIDTH - paddleWidth;
}

static void resetBallOnPaddle()
{
    ballLaunched = false;
    ballX = paddleX + paddleWidth / 2;
    ballY = paddleY + paddleHeight + ballRadius + 2;
}

static void clearPerks()
{
    for(int i = 0; i < ROWS * COLS; i++)
    {
        perks[i].active = false;
        perks[i].type = PERK_NONE;
        perks[i].x = 0;
        perks[i].y = 0;
        perks[i].size = 16;
        perks[i].speed = 2.2f;
    }
}

static void createPerk(float x, float y, int seed)
{
    if(seed % 4 != 0) return;

    for(int i = 0; i < ROWS * COLS; i++)
    {
        if(!perks[i].active)
        {
            perks[i].active = true;
            perks[i].x = x;
            perks[i].y = y;
            perks[i].size = 16;
            perks[i].speed = 2.2f;

            int type = seed % 3;
            if(type == 0) perks[i].type = PERK_EXTRA_LIFE;      // Requirement: extra life
            if(type == 1) perks[i].type = PERK_FAST_BALL;       // Requirement: faster ball
            if(type == 2) perks[i].type = PERK_WIDE_PADDLE;     // Requirement: wider paddle

            break;
        }
    }
}

static void applyPerk(PerkType type)
{
    if(type == PERK_EXTRA_LIFE)
    {
        lives++;
        score += 25;
    }
    else if(type == PERK_FAST_BALL)
    {
        ballDX += (ballDX > 0 ? 1.0f : -1.0f);
        ballDY += (ballDY > 0 ? 1.0f : -1.0f);
        score += 20;
    }
    else if(type == PERK_WIDE_PADDLE)
    {
        paddleWidth += 35;
        if(paddleWidth > 190) paddleWidth = 190;
        clampPaddle();
        score += 20;
    }
}

static void updatePerks()
{
    for(int i = 0; i < ROWS * COLS; i++)
    {
        if(!perks[i].active) continue;

        perks[i].y -= perks[i].speed;

        if(perks[i].y <= paddleY + paddleHeight &&
           perks[i].y + perks[i].size >= paddleY &&
           perks[i].x + perks[i].size >= paddleX &&
           perks[i].x <= paddleX + paddleWidth)
        {
            applyPerk(perks[i].type);
            perks[i].active = false;
        }

        // Remove perk if it falls below the screen.
        if(perks[i].y < -20)
        {
            perks[i].active = false;
        }
    }
}

void loadHighScore()
{
    std::ifstream file("highscore.txt");
    if(file) file >> highScore;
}

void saveHighScore()
{
    if(score > highScore)
    {
        highScore = score;
        std::ofstream file("highscore.txt");
        file << highScore;
    }
}

void initializeGame()
{
    std::srand((unsigned int)std::time(0));

    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        { 
            if(i % 2 == 0 && j % 2 == 0)
            {
                bricks[i][j] = true;
            }
            else
            {
                bricks[i][j] = false;
            }

            brickColor[i][j] = (i + j + level) % 5;
        }
    }

    clearPerks();
    resetBallOnPaddle();
}

void resetGame()
{
    score = 0;
    lives = 3;
    level = 1;
    elapsedTime = 0;
    frameCounter = 0;

    paddleWidth = 110;
    paddleX = WINDOW_WIDTH / 2 - paddleWidth / 2;

    baseBallSpeed = 4.0f;
    ballDX = baseBallSpeed;
    ballDY = baseBallSpeed;

    initializeGame();
}

void startNextLevel()
{
    level++;
    baseBallSpeed += 0.8f;
    ballDX = baseBallSpeed;
    ballDY = baseBallSpeed;
    paddleWidth = 110;
    paddleX = WINDOW_WIDTH / 2 - paddleWidth / 2;
    initializeGame();
}

void updateTimer()
{
    if(currentState == PLAYING)
    {
        frameCounter++;
        if(frameCounter >= 60)
        {
            elapsedTime++;
            frameCounter = 0;

            // Requirement: ball speed gradually increases over time.
            if(elapsedTime % 10 == 0)
            {
                ballDX += (ballDX > 0 ? 0.35f : -0.35f);
                ballDY += (ballDY > 0 ? 0.35f : -0.35f);
            }
        }
    }
}

void updateGame()
{
    clampPaddle();

    if(!ballLaunched)
    {
        resetBallOnPaddle();
        updatePerks();
        return;
    }

    // Move the ball.
    ballX += ballDX;
    ballY += ballDY;

    // Left and right wall collision.
    if(ballX - ballRadius <= 0)
    {
        ballX = ballRadius;
        ballDX = absoluteValue(ballDX);
    }
    if(ballX + ballRadius >= WINDOW_WIDTH)
    {
        ballX = WINDOW_WIDTH - ballRadius;
        ballDX = -absoluteValue(ballDX);
    }

    // Top wall collision.
    if(ballY + ballRadius >= WINDOW_HEIGHT)
    {
        ballY = WINDOW_HEIGHT - ballRadius;
        ballDY = -absoluteValue(ballDY);
    }

    // Paddle collision. Ball angle changes depending on where it hits the paddle.
    if(ballY - ballRadius <= paddleY + paddleHeight &&
       ballY - ballRadius >= paddleY - 5 &&
       ballX >= paddleX &&
       ballX <= paddleX + paddleWidth &&
       ballDY < 0)
    {
        float hitPosition = (ballX - (paddleX + paddleWidth / 2)) / (paddleWidth / 2);
        ballDX = hitPosition * (absoluteValue(ballDX) + 1.0f);
        ballDY = absoluteValue(ballDY);
    }

    // Ball missed by player.
    if(ballY + ballRadius < 0)
    {
        lives--;
        paddleWidth = 110;
        ballDX = baseBallSpeed;
        ballDY = baseBallSpeed;
        resetBallOnPaddle();

        if(lives <= 0)
        {
            PlaySound(TEXT("gameover.wav"), NULL, SND_ASYNC);
            saveHighScore();
            currentState = GAMEOVER;
        }
        return;
    }

    // Brick collision.
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            if(!bricks[i][j]) continue;

            float brickX = BRICK_START_X + j * (BRICK_W + BRICK_GAP);
            float brickY = BRICK_START_Y - i * (BRICK_H + BRICK_GAP);

            bool hit = ballX + ballRadius >= brickX &&
                       ballX - ballRadius <= brickX + BRICK_W &&
                       ballY + ballRadius >= brickY &&
                       ballY - ballRadius <= brickY + BRICK_H;

            if(hit)
            {
                bricks[i][j] = false;
                ballDY = -ballDY;
                score += 10 + (ROWS - i) * 2;
                createPerk(brickX + BRICK_W / 2, brickY, i * COLS + j + score);
                i = ROWS; // stop checking after one hit in this frame
                PlaySound(TEXT("brick_hit.wav"), NULL, SND_ASYNC);
                break;
            }
        }
    }

    updatePerks();

    // Win check.
    bool allBroken = true;
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            if(bricks[i][j]) allBroken = false;
        }
    }

    if(allBroken)
    {
        PlaySound(TEXT("win.wav"), NULL, SND_ASYNC);

        saveHighScore();
        currentState = WIN;
    }
}
