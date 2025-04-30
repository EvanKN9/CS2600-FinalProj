#include <stdio.h>
#include <stdlib.h>

#define cols 15
#define rows 15

void initializeBoard(char board[cols][rows]) {
  for(int i = 0; i < cols; i++){
    for(int j = 0; j < rows; j++){
      if(i==0||j==0||i==cols-1||j==rows-1){
	board[i][j] = '#';
      }
      else {
	board[i][j] = ' ';
      }
    }
  }
}

void printBoard(char board[cols][rows]){
  for(int i = 0; i < cols; i++){
    for(int j = 0; j < rows; j++){
      printf("%c ", board[i][j]);
    }
    printf("\n");
  }
}

int main(){
  char board[cols][rows];

  initializeBoard(board);
  printBoard(board);

  return 0;
}
    
  
	
	  
