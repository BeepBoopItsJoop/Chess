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
          for(size_t i = 1, count = getPieceCount(input.type); i <= count; i++) {
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


     if(!playMove(chessBoard, move, message)) {
          // fprintf(stderr, "Error: Invalid move supplied\n");
          return;
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
          printf("%s %s%s", "It is", board->currentTurn == WHITE ? "White" : "Black", "\'s turn\n");
          gameRound(board, buffer, getInputMove(board, buffer));
     }

}
