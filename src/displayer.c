#include <stdio.h>

#include "displayer.h"

const char* chessPieces[2][7] = {
    // Black pieces
    {" ", "♟", "♜", "♞", "♝", "♛", "♚"},
    // White pieces
     {" ", "♙", "♖", "♘", "♗", "♕", "♔"}
};

void printRow(int digit) {
     printf("%d", digit); 
     for(int i = 0; i < 8; i++) {
          printf("%c", '|'); 
          printf("%c", '_'); 
          printf("%c", '_'); 
          printf("%c", '_'); 
     } 
     printf("%c", '|'); 
     printf("%c", '\n'); 
} 

void printPieces(const Piece pieces[8]) {
     for(int i = 0; i < 8; i++) {
          printf("%c", '|');
          printf("%c", ' ');
          printf("%s", chessPieces[pieces[i].isWhite][pieces[i].type]);
          printf("%c", ' ');
     }
     printf("%c", '|');
     printf("%c", '\n'); 
 }

// void boardPrint(const int board[8][8]) {
void boardPrint(Board* board) {
     printf("%c", ' '); 
     for(int i = 0; i < 8; i++) {
          printf("%c", ' '); 
          printf("%c", '_'); 
          printf("%c", '_'); 
          printf("%c", '_'); 
     } 
     printf("%c", '\n'); 
     for(int i = 7; i >= 0; i--) {
          if(i != 7) {
               printRow(i+2);
          }
          printf("%c", ' ');
          printPieces(&board->boardArray[i][0]);
     }
     printRow(1);

          printf("%s", "   ");
     for(int i = 0; i < 8; i++) {
          printf("%c", 'A'+i); 
          printf("%s", "   ");
     } 
     printf("%c", '\n');

}
