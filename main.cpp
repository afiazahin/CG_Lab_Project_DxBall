#include <GL/glut.h>
#include <cstdlib>
#include "draw.h"
#include "game.h"
#include "menu.h"

#include <windows.h>
#include <mmsystem.h>

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if(currentState == MENU) drawMenu();
    else if(currentState == PLAYING) drawGame();
    else if(currentState == PAUSED) drawPauseScreen();
    else if(currentState == HELP) drawHelpScreen();
    else if(currentState == GAMEOVER) drawGameOver();
    else if(currentState == WIN) drawWinScreen();

    glutSwapBuffers();
}

void update(int value)
{
    if(currentState == PLAYING)
    {
        updateGame();
        updateTimer();
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // around 60 FPS
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 13: // ENTER
            if(currentState == MENU || currentState == GAMEOVER || currentState == WIN)
            {
                PlaySound(TEXT("start.wav"), NULL, SND_ASYNC);

                resetGame();
                currentState = PLAYING;
            }
            break;

        case ' ': // SPACE launches the ball.
            if(currentState == PLAYING) ballLaunched = true;
            break;

        case 'p':
        case 'P':
            if(currentState == PLAYING) currentState = PAUSED;
            break;

        case 'r':
        case 'R':
            if(currentState == PAUSED) currentState = PLAYING;
            break;

        case 'h':
        case 'H':
            currentState = HELP;
            break;

        case 'm':
        case 'M':
            currentState = MENU;
            break;

        case 'n':
        case 'N':
            if(currentState == WIN)
            {
                startNextLevel();
                currentState = PLAYING;
            }
            break;

        case 27: // ESC
            saveHighScore();
            std::exit(0);
            break;
    }
}

void specialKeys(int key, int x, int y)
{
    if(currentState != PLAYING) return;

    if(key == GLUT_KEY_LEFT)  paddleX -= 100;
    if(key == GLUT_KEY_RIGHT) paddleX += 100;

    if(paddleX < 0) paddleX = 0;
    if(paddleX + paddleWidth > WINDOW_WIDTH) paddleX = WINDOW_WIDTH - paddleWidth;
}

void mouseClick(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if(currentState == PLAYING) ballLaunched = true;
    }
}

void mouseMotion(int x, int y)
{
    if(currentState != PLAYING) return;

    // GLUT mouse y is top-to-bottom, but paddle only needs x movement.
    paddleX = x - paddleWidth / 2;

    if(paddleX < 0) paddleX = 0;
    if(paddleX + paddleWidth > WINDOW_WIDTH) paddleX = WINDOW_WIDTH - paddleWidth;
}

void initOpenGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("DX Ball - CSE 426 Computer Graphics Lab");

    initOpenGL();
    loadHighScore();
    initializeGame();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutPassiveMotionFunc(mouseMotion);
    glutMouseFunc(mouseClick);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
