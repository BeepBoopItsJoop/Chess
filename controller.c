#include <stdbool.h>
#include <string.h> // memset
#include <stdio.h>

#include "controller.h"
#include "displayer.h"
#include "validator.h"
#include "inputHandler.h"

void movePiece(Board *chessBoard, Move move, Message message)
{
     message[0] = '\0';

     if (chessBoard == NULL)
     {
          message = "Error: Invalid chess board.\n";
          return;
     }

     if (move.start.X < 1 || move.start.X > 8 || move.start.Y < 1 || move.start.Y > 8 ||
         move.end.X < 1 || move.end.X > 8 || move.end.Y < 1 || move.end.Y > 8)
     {    
          
          message = "Error: Position out of bounds.\n";
          return;
     }

     // For readability
     Piece(*board)[8] = chessBoard->boardArray;
     Piece *startPiece = &board[move.start.Y-1][move.start.X-1];
     Piece *endPiece = &board[move.end.Y-1][move.end.X-1];

     if (startPiece->type == EMPTY)
     {
          message = "Error: No piece at the start location\n";
          return;
     }

     if (endPiece->type != EMPTY)
     {
          sprintf(message, "Piece %d moves from (%d, %d) and takes %d at (%d, %d)\n", startPiece->type, move.start.X, move.start.Y, endPiece->type, move.end.X, move.end.Y);
     } else
          sprintf(message, "Moved piece %d from (%d, %d) to (%d, %d)\n", startPiece->type, move.start.X, move.start.Y, move.end.X, move.end.Y);

     *endPiece = *startPiece;
     *startPiece = (Piece){0};
}

Pos findNthPiece(Board *chessBoard, PieceType type, int n) {
    int count = 0;
    for (size_t y = 1; y < 9; y++) {
        for (size_t x = 1; x < 9; x++) {
            Piece* piece = getPieceAt(chessBoard, x, y);
            if (piece->type == type && piece->isWhite == chessBoard->currentTurn) {
                if (++count == n) {
                    return (Pos){x, y};
                }
            }
        }
    }
    return (Pos){-1, -1};  // Not enough matches found
}

bool isSquareContested(Board* board, Pos square, Message message) {
     for (size_t y = 1; y < 9; y++) {
          for (size_t x = 1; x < 9; x++)
          {
               if (checkValidMove(board, (Move){x, y, square.X, square.Y}, message)) {
                    return true;       
               }
          }
     }
     return false;
}

bool playMove(Board *chessBoard, Move move, Message message)
{
     // TOOD: perform checks here

     // Checks if king is left in check after the move in a temporary board
     Board tempBoard = *chessBoard;
     Message tempBuffer;

     movePiece(&tempBoard, move, tempBuffer);

     if(isSquareContested(&tempBoard, findNthPiece(&tempBoard, KING, 1), tempBuffer)) {
          sprintf(message, "%s", "Invalid move: Exposes king to check\n");
          return false;
     } else {
          movePiece(chessBoard, move, message);
     }

     return true;
}

Move getInputMove(Board* board, Message buffer) {
     // Ask for an input that is a valid move 
     Prompt input;
     Pos start;
     do {
          printf("%s", buffer);
          input = promptMove();
          // Check move for every piece of that type on the board
          int count = getPieceCount(input.type);
          for(size_t i = 1; i <= count; i++) {
               start = findNthPiece(board, input.type, i);
               if(checkValidMove(board, createMove(start.X, start.Y, input.file, input.rank), buffer)) 
                    break;
          }
     } while(!checkValidMove(board, createMove(start.X, start.Y, input.file, input.rank), buffer));

     return createMove(start.X, start.Y, input.file, input.rank);
}

void gameRound(Board *chessBoard, Message message, Move move)
{
     if (chessBoard == NULL)
     {
          fprintf(stderr, "Error: Invalid chess board.\n");
          return;
     }

     // TODO: implement winCondition using turnTeamColor
     if(!playMove(chessBoard, move, message)) {
          fprintf(stderr, "Error: Invalid move supplied");
     }
     boardPrint(chessBoard);
     chessBoard->currentTurn = !chessBoard->currentTurn;
}

void boardInit(Board *chessBoard)
{
     if (chessBoard == NULL)
     {
          printf("Error: Invalid chess board.\n");
          return;
     }

     // Zeroes out Board variable, making all spaces empty
     memset(chessBoard, 0, sizeof(*chessBoard));

     chessBoard->currentTurn = WHITE;
     chessBoard->isGameEnded = false;
     Piece(*board)[8] = chessBoard->boardArray;

     int pieceArrangement[] = {ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK};
     // Sets starting pieces
          // For white
     for (size_t i = 0; i < 8; i++)
          board[0][i] = (Piece){pieceArrangement[i], WHITE};
     for (size_t i = 0; i < 8; i++)
          board[1][i] = (Piece){PAWN, WHITE};

          // For black
     for (size_t i = 0; i < 8; i++)
          board[7][i] = (Piece){pieceArrangement[i], BLACK};
     for (size_t i = 0; i < 8; i++)
          board[6][i] = (Piece){PAWN, BLACK};
}

// void tempDebugFunction(Board* board, Message message) {
     // playMove(board, createMove(0, 1, 0, 2), message);
     // playMove(board, createMove(0, 0, 0, 1), message);
     // playMove(board, createMove(0, 1, 1, 1), message);
     // playMove(board, createMove(1, 1, 1, 2), message);
     // playMove(board, createMove(0, 1, 1, 1), message);
     // playMove(board, createMove(1, 2, 1, 3), message); // Move 5
     // playMove(board, createMove(1, 1, 1, 2), message); // Move 6
     // playMove(board, createMove(1, 2, 2, 2), message); // Move 7
     // playMove(board, createMove(2, 2, 2, 5), message); // Move 8
     // playMove(board, createMove(2, 5, 7, 5), message); // Move 9
     // playMove(board, createMove(7, 5, 7, 6), message); // Move 10
     // playMove(board, createMove(7, 6, 6, 7), message); // Move 11 
     // playMove(board, createMove(7, 6, 7, 7), message); // Move 12 error
     // playMove(board, createMove(7, 7, 6, 7), message); // Move 13
     // playMove(board, createMove(6, 7, 4, 7), message); // Move 14
     // playMove(board, createMove(2, 1, 2, 3), message); // Move 15
     // playMove(board, createMove(6, 7, 5, 7), message); // Move 16
     // playMove(board, createMove(5, 7, 5, 3), message); // Move 17
     // playMove(board, createMove(5, 7, 5, 6), message); // Move 18
     // playMove(board, createMove(5, 6, 5, 3), message); // Move 19
     // playMove(board, createMove(5, 3, 0, 3), message); // Move 20
     // playMove(board, createMove(5, 3, 2, 3), message); // Move 21
     // playMove(board, createMove(5, 3, 3, 3), message); // Move 22
     // playMove(board, createMove(3, 3, 3, 2), message); // Move 23
     // playMove(board, createMove(1, 0, 2, 2), message);
     // playMove(board, createMove(2, 2, 4, 1), message);
     // playMove(board, createMove(2, 2, 1, 4), message);
     // playMove(board, createMove(1, 4, 0, 6), message);
     // playMove(board, createMove(3, 6, 3, 5), message);
     // playMove(board, createMove(2, 7, 5, 4), message);
     // playMove(board, createMove(4, 7, 5, 6), message);      // King
     // playMove(board, createMove(5, 6, 4, 5), message); 
     // playMove(board, createMove(4, 5, 3, 5), message); 
     // playMove(board, createMove(2, 3, 2, 4), message); // moves pawn
     // playMove(board, createMove(3, 2, 2, 2), message); // hides rook
     // playMove(board, createMove(4, 5, 3, 4), message); // exposes king
     // playMove(board, createMove(2, 2, 7, 2), message); // hides rook, again
     // playMove(board, createMove(3, 4, 2, 3), message); // exposes king, again
     // playMove(board, createMove(2, 3, 4, 1), message);
     // playMove(board, createMove(3, 7, 2, 7), message);     // Queen
     // playMove(board, createMove(2, 7, 6, 3), message);
     // playMove(board, createMove(2, 7, 4, 5), message);
     // playMove(board, createMove(4, 5, 4, 3), message);
     // playMove(board, createMove(4, 3, 3, 2), message);
     // playMove(board, createMove(3, 2, 5, 0), message);
     // playMove(board, createMove(2, 1, 2, 3), message);
     // playMove(board, createMove(4, 1, 4, 3), message);
     // playMove(board, createMove(4, 3, 4, 5), message);
     // playMove(board, createMove(3, 0, 4, 1), message);
     // playMove(board, createMove(4, 1, 3, 2), message);
     // playMove(board, createMove(2, 3, 2, 2), message);
     // playMove(board, createMove(3, 2, 3, 3), message);
     // playMove(board, createMove(6, 0, 5, 2), message);
     // playMove(board, createMove(2, 3, 3, 3), message);
// }

void gameInit(Board *board)
{
     // TODO: outline what goes here -> ... nothing?
     boardInit(board);

}

void gameStart(Board* board) {
     if (board == NULL)
     {
          printf("Error: Invalid chess board.\n");
          return;
     }

     gameInit(board);
     Message buffer;

     boardPrint(board);
     while(!board->isGameEnded) {
          gameRound(board, buffer, getInputMove(board, buffer));
     }

}
