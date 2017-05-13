#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>

#define MARGIN 2
#define PADDLESIZE 4
#define BALLSTEP 0.1
#define BACKGROUND ' '

#define GOALCHAR '#'
#define PADDLECHAR '|'
#define BALLCHAR '0'
#define EMPTYCHAR ' '

    
typedef struct Point {
    float x;
    float y;
} Point;

typedef struct Ball {
    int direction;
    float x;
    float y;
} Ball;

typedef struct Pong {
    int sizeX;
    int sizeY;
    int paddle1;//paddle coordinates above lowest point
    int paddle2;
    Ball ball;
} Pong;


void initialise(Pong *g, int x, int y) {
    int halfY = y/2;
    int halfX = x/2;
    
    (*g).sizeX = x;
    (*g).sizeY = y;
    (*g).paddle1 = halfY - PADDLESIZE/2;
    (*g).paddle2 = (*g).paddle1;
    (*g).ball.x = halfX;
    (*g).ball.y = halfY;
    (*g).ball.direction = 1;
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
                    ( y > (*g).paddle1 || y > (*g).paddle2 ) &&
                    ( y < (*g).paddle1 + PADDLESIZE ||
                      y < (*g).paddle2 + PADDLESIZE )
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
            } else if(( x == MARGIN || x == maxX - 1  - MARGIN ) && 
                    ( y >= (*g).paddle1 || y >= (*g).paddle2 ) &&
                    ( y < (*g).paddle1 + PADDLESIZE ||
                      y < (*g).paddle2 + PADDLESIZE )) {
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
    float step = BALLSTEP;
    (*g).ball.x += step;
}

void update(Pong *g) {
    moveBall(g);
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
        draw(game);
        update(game);
        usleep(16*1000);
    }

    getch();
    endwin();
    return 0;
}
