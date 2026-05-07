#include <GL/glut.h>
#include <iostream>

#include "draw.h"
#include "game.h"
#include "menu.h"

using namespace std;

int windowWidth = 800;
int windowHeight = 600;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    switch(currentState)
    {
        case MENU:
            drawMenu();
            break;

        case PLAYING:
            drawGame();
            break;

        case PAUSED:
            drawPauseScreen();
            break;

        case HELP:
            drawHelpScreen();
            break;

        case GAMEOVER:
            drawGameOver();
            break;

        case WIN:
            drawWinScreen();
            break;
    }

    glutSwapBuffers();
}

void update(int value)
{
    if(currentState == PLAYING)
    {
        updateGame();
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 13: // ENTER
            if(currentState == MENU)
            {
                resetGame();
                currentState = PLAYING;
            }
            break;

        case 'p':
        case 'P':
            if(currentState == PLAYING)
                currentState = PAUSED;
            break;

        case 'r':
        case 'R':
            if(currentState == PAUSED)
                currentState = PLAYING;
            break;

        case 'h':
        case 'H':
            currentState = HELP;
            break;

        case 27: // ESC
            exit(0);
            break;
    }
}

void specialKeys(int key, int x, int y)
{
    if(currentState != PLAYING)
        return;

    switch(key)
    {
        case GLUT_KEY_LEFT:
            paddleX -= 25;

            if(paddleX < 0)
                paddleX = 0;

            break;

        case GLUT_KEY_RIGHT:
            paddleX += 25;

            if(paddleX + paddleWidth > windowWidth)
                paddleX = windowWidth - paddleWidth;

            break;
    }
}

void mouseMotion(int x, int y)
{
    if(currentState != PLAYING)
        return;

    paddleX = x - paddleWidth / 2;

    if(paddleX < 0)
        paddleX = 0;

    if(paddleX + paddleWidth > windowWidth)
        paddleX = windowWidth - paddleWidth;
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0, windowWidth, 0, windowHeight);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(windowWidth, windowHeight);

    glutCreateWindow("DX BALL GAME");

    init();

    initializeGame();

    glutDisplayFunc(display);

    glutKeyboardFunc(keyboard);

    glutSpecialFunc(specialKeys);

    glutPassiveMotionFunc(mouseMotion);

    glutTimerFunc(16, update, 0);

    glutMainLoop();

    return 0;
}