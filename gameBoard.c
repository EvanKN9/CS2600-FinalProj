// Group 1: Cristian Gramada, Ethan Wong, Evan Nguyen, Justin Nacpil

#include <stdio.h>
#include <stdlib.h>

#define COLS 20
#define ROWS 20

char board[ROWS][COLS];


void initializeBoard(void) {
  for(int y = 0; y < ROWS; y++){
    for(int x = 0; x < COLS; x++){
      if (y==0 || x==0 || y==ROWS-1 || x==COLS-1)
        board[y][x] = '#';
      else
        board[y][x] = ' ';
    }
  }
}

void printBoard(void){
  for(int y = 0; y < ROWS; y++){
    for(int x = 0; x < COLS; x++){
      printf("%c ", board[y][x]);
    }
    putchar('\n');
  }
}

int snakeX = 10, snakeY = 10;

void drawSnake(void) {
  board[snakeY][snakeX] = '@';
}

void moveSnake(int dx, int dy) {
  board[snakeY][snakeX] = ' ';
  snakeX += dx;
  snakeY += dy;

}



void read_keyboard(){
	int ch = getchar();
	switch(ch){
		case 'w':
			moveSnake(0, -1); // Move up
			break;
		case 's':
			moveSnake(0, 1); // Move down
			break;	
		case 'a':
			moveSnake(-1, 0); // Move left
			break;
		case 'd':
			moveSnake(1, 0); // Move right
			break;
	}
}



int main(void) {
  initializeBoard();
  drawSnake();
  printBoard();

  return 0;
}

    
  
	
	  
