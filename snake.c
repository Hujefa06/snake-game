#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH 40
#define HEIGHT 20

int gameOver;
int score;
int highScore = 0;
int snakeX, snakeY;
int ballX, ballY;
int tailX[100], tailY[100];
int tailLength;
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum Direction dir;

void loadHighScore() {
    FILE *file = fopen("highscore.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &highScore);
        fclose(file);
    }
}

void saveHighScore() {
    if (score > highScore) {
        highScore = score;
        FILE *file = fopen("highscore.txt", "w");
        if (file != NULL) {
            fprintf(file, "%d", highScore);
            fclose(file);
        }
    }
}

void setup() {
    gameOver = 0;
    dir = STOP;
    snakeX = WIDTH / 2;
    snakeY = HEIGHT / 2;
    srand(time(NULL));
    ballX = rand() % WIDTH;
    ballY = rand() % HEIGHT;
    score = 0;
    tailLength = 0;
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void draw() {
    gotoxy(0, 0);
    
    // Top wall
    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");
    
    // Game area
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0)
                printf("#");
            
            if (i == snakeY && j == snakeX)
                printf("O");
            else if (i == ballY && j == ballX)
                printf("*");
            else {
                int isTail = 0;
                for (int k = 0; k < tailLength; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printf("o");
                        isTail = 1;
                    }
                }
                if (!isTail)
                    printf(" ");
            }
            
            if (j == WIDTH - 1)
                printf("#");
        }
        printf("\n");
    }
    
    // Bottom wall
    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");
    
    printf("Score: %d\n", score);
    printf("High Score: %d\n", highScore);
    printf("Use W/A/S/D to move, X to quit\n");
}

void input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a':
            case 'A':
                if (dir != RIGHT)
                    dir = LEFT;
                break;
            case 'd':
            case 'D':
                if (dir != LEFT)
                    dir = RIGHT;
                break;
            case 'w':
            case 'W':
                if (dir != DOWN)
                    dir = UP;
                break;
            case 's':
            case 'S':
                if (dir != UP)
                    dir = DOWN;
                break;
            case 'x':
            case 'X':
                gameOver = 1;
                break;
        }
    }
}

void logic() {
    // Move tail
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = snakeX;
    tailY[0] = snakeY;
    
    for (int i = 1; i < tailLength; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    
    // Move snake head
    switch (dir) {
        case LEFT:
            snakeX--;
            break;
        case RIGHT:
            snakeX++;
            break;
        case UP:
            snakeY--;
            break;
        case DOWN:
            snakeY++;
            break;
        default:
            break;
    }
    
    // Check wall collision
    if (snakeX < 0 || snakeX >= WIDTH || snakeY < 0 || snakeY >= HEIGHT)
        gameOver = 1;
    
    // Check self collision
    for (int i = 0; i < tailLength; i++) {
        if (tailX[i] == snakeX && tailY[i] == snakeY)
            gameOver = 1;
    }
    
    // Check if snake ate the ball
    if (snakeX == ballX && snakeY == ballY) {
        score += 10;
        ballX = rand() % WIDTH;
        ballY = rand() % HEIGHT;
        tailLength++;
    }
}

int main() {
    loadHighScore();
    setup();
    hideCursor();
    
    while (!gameOver) {
        draw();
        input();
        logic();
        Sleep(220);
    }
    
    saveHighScore();
    
    system("cls");
    printf("\n\n");
    printf("  ===========================\n");
    printf("       GAME OVER!\n");
    printf("  ===========================\n");
    printf("    Your Score: %d\n", score);
    printf("    High Score: %d\n", highScore);
    printf("  ===========================\n\n");
    
    return 0;
}