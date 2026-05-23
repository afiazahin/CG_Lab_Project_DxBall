#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include "draw.h"
#include "game.h"

static const float PI = 3.1415926f;
static const float BRICK_W = 70.0f;
static const float BRICK_H = 22.0f;
static const float BRICK_GAP = 5.0f;
static const float BRICK_START_X = 25.0f;
static const float BRICK_START_Y = 505.0f;

void drawText(float x, float y, const char* text)
{
    glRasterPos2f(x, y);
    while(*text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

static void setBrickColor(int colorId)
{
    // Bright neon colors, similar to classic DX Ball screens.
    if(colorId == 0) glColor3f(1.0f, 0.10f, 0.25f);      // red/pink
    else if(colorId == 1) glColor3f(1.0f, 0.55f, 0.05f); // orange
    else if(colorId == 2) glColor3f(0.95f, 0.00f, 0.95f);// magenta
    else if(colorId == 3) glColor3f(0.10f, 0.45f, 1.0f); // blue
    else glColor3f(1.0f, 0.95f, 0.10f);                  // yellow
}

void drawBackground()
{
    // Dark background with purple game area to make the bricks look colorful.
    glBegin(GL_QUADS);
        glColor3f(0.01f, 0.01f, 0.06f); glVertex2f(0, 0);
        glColor3f(0.02f, 0.00f, 0.12f); glVertex2f(WINDOW_WIDTH, 0);
        glColor3f(0.12f, 0.00f, 0.20f); glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
        glColor3f(0.02f, 0.00f, 0.10f); glVertex2f(0, WINDOW_HEIGHT);
    glEnd();

    // Border line of the playground.
    glColor3f(0.7f, 0.7f, 1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(5, 5);
        glVertex2f(WINDOW_WIDTH - 5, 5);
        glVertex2f(WINDOW_WIDTH - 5, WINDOW_HEIGHT - 5);
        glVertex2f(5, WINDOW_HEIGHT - 5);
    glEnd();
}

void drawPaddle()
{
    // Paddle body.
    glColor3f(0.0f, 0.95f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(paddleX, paddleY);
        glVertex2f(paddleX + paddleWidth, paddleY);
        glVertex2f(paddleX + paddleWidth, paddleY + paddleHeight);
        glVertex2f(paddleX, paddleY + paddleHeight);
    glEnd();

    // Paddle highlight.
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
        glVertex2f(paddleX + 8, paddleY + paddleHeight - 3);
        glVertex2f(paddleX + paddleWidth - 8, paddleY + paddleHeight - 3);
    glEnd();
}

void drawBall()
{
    // Ball outer ring.
    glColor3f(0.3f, 0.8f, 1.0f);
    glBegin(GL_POLYGON);
    for(int i = 0; i < 180; i+=2)
    {
        float angle = i * PI / 180.0f;
        glVertex2f(ballX + std::cos(angle) * (ballRadius + 2),
                   ballY + std::sin(angle) * (ballRadius + 2));
    }
    glEnd();

    // Ball center.
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    for(int i = 0; i < 360; i++)
    {
        float angle = i * PI / 180.0f;
        glVertex2f(ballX + std::cos(angle) * ballRadius,
                   ballY + std::sin(angle) * ballRadius);
    }
    glEnd();
}

void drawBricks()
{
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            if(!bricks[i][j]) continue;

            float x = BRICK_START_X + j * (BRICK_W + BRICK_GAP);
            float y = BRICK_START_Y - i * (BRICK_H + BRICK_GAP);

            // Draw colorful brick body.
            setBrickColor(brickColor[i][j]);
            glBegin(GL_QUADS);
                glVertex2f(x, y);
                glVertex2f(x + BRICK_W, y);
                glVertex2f(x + BRICK_W, y + BRICK_H);
                glVertex2f(x, y + BRICK_H);
            glEnd();

            // Draw small golden center, like classic DX Ball block style.
            glColor3f(1.0f, 0.82f, 0.25f);
            glBegin(GL_QUADS);
                glVertex2f(x + 18, y + 6);
                glVertex2f(x + BRICK_W - 18, y + 6);
                glVertex2f(x + BRICK_W - 18, y + BRICK_H - 6);
                glVertex2f(x + 18, y + BRICK_H - 6);
            glEnd();

            // Brick outline.
            glColor3f(0.05f, 0.02f, 0.08f);
            glBegin(GL_LINE_LOOP);
                glVertex2f(x, y);
                glVertex2f(x + BRICK_W, y);
                glVertex2f(x + BRICK_W, y + BRICK_H);
                glVertex2f(x, y + BRICK_H);
            glEnd();
        }
    }
}

void drawPerks()
{
    for(int i = 0; i < ROWS * COLS; i++)
    {
        if(!perks[i].active) continue;

        if(perks[i].type == PERK_EXTRA_LIFE) glColor3f(0.0f, 1.0f, 0.2f);
        else if(perks[i].type == PERK_FAST_BALL) glColor3f(1.0f, 0.1f, 0.1f);
        else if(perks[i].type == PERK_WIDE_PADDLE) glColor3f(0.1f, 0.6f, 1.0f);
        else glColor3f(1.0f, 1.0f, 1.0f);

        float s = perks[i].size;
        glBegin(GL_QUADS);
            glVertex2f(perks[i].x, perks[i].y);
            glVertex2f(perks[i].x + s, perks[i].y);
            glVertex2f(perks[i].x + s, perks[i].y + s);
            glVertex2f(perks[i].x, perks[i].y + s);
        glEnd();

        glColor3f(0, 0, 0);
        if(perks[i].type == PERK_EXTRA_LIFE) drawText(perks[i].x + 3, perks[i].y + 2, "+");
        if(perks[i].type == PERK_FAST_BALL) drawText(perks[i].x + 4, perks[i].y + 2, "F");
        if(perks[i].type == PERK_WIDE_PADDLE) drawText(perks[i].x + 3, perks[i].y + 2, "W");
    }
}

void drawGame()
{
    drawBackground();
    drawBricks();
    drawPerks();
    drawPaddle();
    drawBall();

    glColor3f(1.0f, 1.0f, 1.0f);

    char text[80];
    sprintf(text, "Score: %d", score);
    drawText(20, 20, text);

    sprintf(text, "Lives: %d", lives);
    drawText(180, 20, text);

    sprintf(text, "Time: %ds", elapsedTime);
    drawText(320, 20, text);

    sprintf(text, "Level: %d", level);
    drawText(470, 20, text);

    sprintf(text, "High Score: %d", highScore);
    drawText(600, 20, text);

    if(!ballLaunched)
    {
        glColor3f(1.0f, 1.0f, 0.3f);
        drawText(270, 75, "Left click or SPACE to launch the ball");
    }
}
