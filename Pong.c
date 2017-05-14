#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
//config
#define MARGIN 2
#define PADDLESIZE 4
#define BALLSTEP 0.1
//graphics
#define BACKGROUND ' '
#define GOALCHAR '#'
#define PADDLECHAR '|'
#define BALLCHAR '0'
#define EMPTYCHAR ' '
//controls
#define UP1 'w'
#define DN1 's'
#define UP2 'i' 
#define DN2 'k'
#define EXIT ' '

typedef struct Ball {
    int direction;
    float x;
    float y;
} Ball;

typedef struct Pong {
    int sizeX;
    int sizeY;
    int paddles[2];//paddle coordinates above lowest point
    Ball ball;
} Pong;


void initialise(Pong *g, int x, int y) {
    int halfY = y/2;
    int halfX = x/2;
    
    (*g).sizeX = x;
    (*g).sizeY = y;
    (*g).paddles[0] = halfY - PADDLESIZE/2;
    (*g).paddles[1] = (*g).paddles[0];
    (*g).ball.x = halfX;
    (*g).ball.y = halfY;
    (*g).ball.direction = 1;
}
bool isPaddle(Pong *g, float x, float y) {
    x = floor(x);
    y = floor(y);
    return ( x == MARGIN || x == (*g).sizeX - 1  - MARGIN ) && 
            ( y >= (*g).paddles[0] || y >= (*g).paddles[1] ) &&
            ( y < (*g).paddles[0] + PADDLESIZE ||
              y < (*g).paddles[1] + PADDLESIZE );
}

void draw(Pong *g) {
    int maxX = (*g).sizeX;
    int maxY = (*g).sizeY;

    char screen[maxX][maxY];
    char nextChar;
    /* 
    //build up array of screen
    for(int x = 0; x < maxX ; x++) {
        for(int y = 0; y < maxY; y++) {
            if(x == 0 || x == maxX - 1) {
                nextChar = GOALCHAR;
            } else if(x == MARGIN || x == maxX - 1  - MARGIN && 
                    ( y > (*g).paddles[0] || y > (*g).paddles[1] ) &&
                    ( y < (*g).paddles[0] + PADDLESIZE ||
                      y < (*g).paddles[1] + PADDLESIZE )
                    ) {
                nextChar = PADDLECHAR;
            } else if(x == (*g).ball.x && y == (*g).ball.y) {
                nextChar = BALLCHAR;
            } else {
                nextChar = EMPTYCHAR;
            }

            screen[x][y] = nextChar;
        }
    }*/
    
    for(int y = 0; y < maxY; y++) {
        for(int x = 0; x < maxX ; x++) {
            if(x == 0 || x == maxX - 1) {
                nextChar = GOALCHAR;
            } else if(isPaddle(g, x, y)) {
                nextChar = PADDLECHAR;
            } else if(x == floor((*g).ball.x) && y == floor((*g).ball.y)) {
                nextChar = BALLCHAR;
            } else { 
                nextChar = EMPTYCHAR;
            }

            screen[x][y] = nextChar;
        }
    }
    
    
    //print out array
    move(0, 0);
    for(int y = 0; y < maxY; y++) {
        for(int x = 0; x < maxX ; x++) {
            printw("%c", screen[x][y]);
        }
    }
    refresh();
}

void moveBall(Pong *g) {
    float step = BALLSTEP * (*g).ball.direction;
    float nextX = (*g).ball.x + step;

    if(isPaddle(g, nextX, (*g).ball.y)) {
        step *= -1;
        (*g).ball.direction *= -1;
    }

    (*g).ball.x += step;
}

void update(Pong *g) {
    moveBall(g);
}

void movePaddle(Pong *g, int p, int n) {
    if((*g).paddles[p] + PADDLESIZE >= (*g).sizeY && n == 1) {
        return;
    } else if((*g).paddles[p] <= 0 && n == -1) {
        return;
    } else {
        (*g).paddles[p] += n;
    }
}
    
void processInput(Pong *g) {
    char c;
    c = getchar();
    while(c >= 0) {
        switch(c) {
            case UP1 :
                movePaddle(g, 0, 1);
                break;
            case DN1 :
                movePaddle(g, 0, -1);
                break;
            case UP2 :
                movePaddle(g, 1, 1);
                break;
            case DN2 :
                movePaddle(g, 1, -1);
                break;
            case EXIT:
                exit(EXIT_SUCCESS);
                break;
        }
    }
}

int main() {
    int w_x;
    int w_y;

    Pong *game = malloc(sizeof(Pong));

    initscr();
    //raw();
    noecho();

    getmaxyx(stdscr, w_y, w_x);

    initialise(game, w_x, w_y);
   
    while(1) { 
        processInput(game);
        draw(game);
        update(game);
        usleep(16*1000);
    }
    endwin();
    return 0;
}
