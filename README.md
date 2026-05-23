# CG_Lab_Project_DX-BALL
computer graphics final lab project - DX Ball
DX Ball Fixed Project - How to Run

Files:
- main.cpp
- game.cpp / game.h
- draw.cpp / draw.h
- menu.cpp / menu.h

Visual Studio:
1. Create Empty C++ Project.
2. Add all .cpp and .h files.
3. Configure FreeGLUT include/lib folders.
4. Link: freeglut.lib opengl32.lib glu32.lib
5. Copy freeglut.dll beside the generated .exe if needed.
6. Run.

Code::Blocks / MinGW example:
g++ main.cpp game.cpp draw.cpp menu.cpp -o DXBall.exe -lfreeglut -lopengl32 -lglu32

Controls:
ENTER = Start / Restart
SPACE or Left Mouse Click = Launch ball
LEFT/RIGHT = Move paddle
Mouse Move = Move paddle
P = Pause
R = Resume
H = Help
M = Main Menu
N = Next Level after win
ESC = Exit

Implemented Sir's Requirements:
- Playable DX Ball base game
- Menu page with start/help/exit and high score
- Pause/resume/exit anytime
- Keyboard and mouse paddle control
- 3 initial lives
- Score, time, level, life display
- Ball speed increases gradually over time
- At least 3 perks: extra life, faster ball, wider paddle
- Colorful DX Ball style bricks
