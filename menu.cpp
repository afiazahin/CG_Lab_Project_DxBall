#include <GL/glut.h>
#include <cstdio>
#include "menu.h"
#include "draw.h"
#include "game.h"

static void drawPanel(float x1, float y1, float x2, float y2)
{
    glColor3f(0.08f, 0.02f, 0.18f);
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();

    glColor3f(0.8f, 0.3f, 1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}

static void drawTitle()
{
    glColor3f(1.0f, 0.2f, 0.9f);
    drawText(315, 485, "DX BALL");
    glColor3f(1.0f, 0.85f, 0.2f);
    drawText(278, 455, "Computer Graphics Lab Project");
}

void drawMenu()
{
    // background
    glColor3f(0.02, 0.0, 0.08);
    glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(800, 0);
        glVertex2f(800, 600);
        glVertex2f(0, 600);
    glEnd();

    // outer border
    glColor3f(0.45, 0.0, 0.90);
    glBegin(GL_LINE_LOOP);
        glVertex2f(8, 8);
        glVertex2f(792, 8);
        glVertex2f(792, 592);
        glVertex2f(8, 592);
    glEnd();

    // menu panel
    glColor3f(0.07, 0.0, 0.15);
    glBegin(GL_QUADS);
        glVertex2f(230, 120);
        glVertex2f(570, 120);
        glVertex2f(570, 480);
        glVertex2f(230, 480);
    glEnd();

    // panel border
    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(230, 120);
        glVertex2f(570, 120);
        glVertex2f(570, 480);
        glVertex2f(230, 480);
    glEnd();

    glColor3f(1.0, 0.0, 1.0);
    drawText(355, 430, "DX BALL");

    glColor3f(1.0, 1.0, 1.0);
    drawText(300, 330, "ENTER  - Start New Game");
    drawText(300, 290, "H      - Help Menu");
    drawText(300, 250, "ESC    - Exit Game");

    char hs[50];
    glColor3f(0.0, 1.0, 1.0);
    sprintf(hs, "High Score: %d", highScore);
    drawText(325, 190, hs);
  
}

void drawPauseScreen()
{
    drawGame();
    drawPanel(235, 235, 565, 375);

    glColor3f(1.0f, 1.0f, 0.2f);
    drawText(340, 330, "GAME PAUSED");
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(300, 290, "R - Resume");
    drawText(300, 260, "M - Main Menu");
}

void drawHelpScreen()
{
    drawBackground();
    drawPanel(190, 100, 610, 520);

    glColor3f(1.0f, 0.85f, 0.2f);
    drawText(375, 485, "HELP");

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(230, 435, "Goal: Break all colorful bricks.");
    drawText(230, 395, "LEFT / RIGHT arrow : Move paddle");
    drawText(230, 365, "Mouse movement     : Move paddle");
    drawText(230, 335, "Left click / SPACE : Launch ball");
    drawText(230, 305, "P                  : Pause game");
    drawText(230, 275, "R                  : Resume game");
    drawText(230, 245, "M                  : Main menu");
    drawText(230, 215, "ESC                : Exit");

    glColor3f(0.0f, 1.0f, 0.2f);
    drawText(200, 155, "+ = Extra life");

    glColor3f(1.0f, 0.1f, 0.1f);
    drawText(320, 155, "F = Faster ball");

    glColor3f(0.1f, 0.6f, 1.0f);
    drawText(455, 155, "W = Wide paddle");
}

void drawGameOver()
{
    drawBackground();
    drawPanel(235, 215, 565, 405);

    glColor3f(1.0f, 0.1f, 0.1f);
    drawText(335, 360, "GAME OVER");

    char text[80];
    glColor3f(1.0f, 1.0f, 1.0f);
    sprintf(text, "Final Score: %d", score);
    drawText(320, 320, text);
    sprintf(text, "High Score: %d", highScore);
    drawText(320, 290, text);

    drawText(280, 250, "ENTER - Play Again");
    drawText(280, 225, "M     - Main Menu");
}

void drawWinScreen()
{
    drawBackground();
    drawPanel(240, 180, 560, 430);

    glColor3f(0.2f, 1.0f, 0.2f);
    drawText(355, 385, "YOU WIN!");

    char text[80];

    glColor3f(1.0f, 1.0f, 1.0f);
    sprintf(text, "Score: %d", score);
    drawText(330, 340, text);

    sprintf(text, "Time: %d seconds", elapsedTime);
    drawText(330, 310, text);

    glColor3f(1.0f, 0.85f, 0.2f);
    drawText(300, 265, "N     - Next Level");
    drawText(300, 230, "M     - Main Menu");
    drawText(300, 195, "ENTER - Restart From Level 1");
}
