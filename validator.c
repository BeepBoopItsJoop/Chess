#include "piece.h"
#include "validator.h"

#include <stdlib.h> // for abs
#include <stdio.h>


bool checkValidMovePawn(const Board *chessBoard, Move m, Message message)
{
     // For readability
     Piece startPiece = getPieceAt(chessBoard, m.start.X, m.start.Y);
     Piece endPiece = getPieceAt(chessBoard, m.end.X, m.end.Y);

     // Account which way is forward (black or white)
     int forward = (startPiece.isWhite ? 1 : -1);

     // If Forward 1 move
     if ((m.end.X == m.start.X) && (m.end.Y - m.start.Y == (1 * forward)))
     {
          // Check if piece is there
          if (endPiece.type != EMPTY)
          {
               sprintf(message, "%s", "Invalid move: Piece in the way\n");
               return false;
          }
          return true;
     }

     // If Forward 2 moves
     if ((m.end.X == m.start.X) && (m.end.Y - m.start.Y == (2 * forward)))
     {
          // Check if starting position
          if (!
          (startPiece.isWhite && m.start.Y == 2 || !startPiece.isWhite && m.start.Y == 7))
          {
               sprintf(message, "%s", "Invalid move: Pawn must be in starting location to move 2 squares forward\n");
               return false;
          }
          // check if piece is directly infront
          if (getPieceAt(chessBoard, m.end.X, m.start.Y + (1 * forward)).type != EMPTY)
          {
               sprintf(message, "%s", "Invalid move: Piece directly infront\n");
               return false;
          }
          // Check if piece is there
          if (endPiece.type != EMPTY)
          {
               sprintf(message, "%s", "Invalid move: Piece already there\n");
               return false;
          }
          return true;
     }

     // If 1 column to either side
     if ((abs(m.end.X - m.start.X) == 1) && (abs(m.end.Y - m.start.Y) == 1))
     {
          // Check if there is a piece there
          if (endPiece.type == EMPTY)
          {
               sprintf(message, "%s", "Invalid move: There is no piece to take\n");
               return false;
          }
          // Check if piece is same team
          if (endPiece.isWhite == startPiece.isWhite)
          {
               sprintf(message, "%s", "Invalid move: Piece of same colour is there\n");
               return false;
          }
          return true;
     }

     // Else invalid
     sprintf(message, "%s", "Invalid move: Pawn can't move that way\n");
     return false;
}
bool checkValidMoveRook(const Board *chessBoard, Move m, Message message)
{
     // For readability
     Piece startPiece = getPieceAt(chessBoard, m.start.X, m.start.Y);
     Piece endPiece = getPieceAt(chessBoard, m.end.X, m.end.Y);

     if (m.start.X != m.end.X && m.start.Y != m.end.Y)
     {
          sprintf(message, "%s", "Invalid move: Rook can't move diagonally\n");
          return false;
     }

     if (m.start.Y == m.end.Y)
     {
          int direction = (m.start.X < m.end.X) ? 1 : -1;
          // i = Startx + direction - To not check the starting location
          // i != endX - Check every square except the last one
          for (size_t x = m.start.X + direction; x != m.end.X; x += direction)
          {
               if (getPieceAt(chessBoard, x, m.start.Y).type != EMPTY)
               {
                    sprintf(message, "%s", "Invalid move: There is a piece in the way\n");
                    return false;
               }
          }
     }
     if (m.start.X == m.end.X)
     {
          int direction = (m.start.Y < m.end.Y) ? 1 : -1;
          // i = StartY + direction - To not check the starting location
          // i != endY - Check every square except the last one
          for (size_t y = m.start.Y + direction; y != m.end.Y; y += direction)
          {
               if (getPieceAt(chessBoard, m.start.X, y).type != EMPTY)
               {
                    sprintf(message, "%s", "Invalid move: There is a piece in the way\n");
                    return false;
               }
          }
     }

     // Check if there is a piece at the end that can be taken
     if (endPiece.type != EMPTY && endPiece.isWhite == startPiece.isWhite)
     {
          sprintf(message, "%s", "Invalid move: Can't take same team's piece\n");
          return false;
     }

     return true;
}
bool checkValidMoveKnight(const Board *chessBoard, Move m, Message message)
{
     // For readability
     Piece startPiece = getPieceAt(chessBoard, m.start.X, m.start.Y);
     Piece endPiece = getPieceAt(chessBoard, m.end.X, m.end.Y);

     // -+2x && -+1y || -=1x -+2y
     if (!(
             (abs(m.end.X - m.start.X) == 2 && abs(m.end.Y - m.start.Y) == 1) ||
             (abs(m.end.X - m.start.X) == 1 && abs(m.end.Y - m.start.Y) == 2)))
     {
          sprintf(message, "%s", "Invalid move: Knight can't move that way\n");
          return false;
     }

     // Check if there is a piece at the end that can be taken
     if (endPiece.type != EMPTY && endPiece.isWhite == startPiece.isWhite)
     {
          sprintf(message, "%s", "Invalid move: Can't take same team's piece\n");
          return false;
     }

     return true;
}
bool checkValidMoveBishop(const Board *chessBoard, Move m, Message message)
{
     // For readability
     Piece startPiece = getPieceAt(chessBoard, m.start.X, m.start.Y);
     Piece endPiece = getPieceAt(chessBoard, m.end.X, m.end.Y);

     if (abs(m.end.X - m.start.X) != abs(m.end.Y - m.start.Y))
     {
          sprintf(message, "%s", "Invalid move: Bishop must move diagonally\n");
          return false;
     }

     int xDirection = (m.end.X > m.start.X) ? 1 : -1;
     int yDirection = (m.end.Y > m.start.Y) ? 1 : -1;

     int x = m.start.X + xDirection;
     int y = m.start.Y + yDirection;
     while (x != m.end.X && y != m.end.Y)
     {
          if (getPieceAt(chessBoard, x, y).type != EMPTY)
          {
               
               sprintf(message, "%s", "Invalid move: There is a piece in the way\n");
               return false;
          }
          x += xDirection;
          y += yDirection;
     }

     if (endPiece.type != EMPTY && endPiece.isWhite == startPiece.isWhite)
     {
          sprintf(message, "%s", "Invalid move: Can't take same team's piece\n");
          return false;
     }

     return true;
}
bool checkValidMoveKing(const Board *chessBoard, Move m, Message message)
{
     // For readability
     Piece startPiece = getPieceAt(chessBoard, m.start.X, m.start.Y);
     Piece endPiece = getPieceAt(chessBoard, m.end.X, m.end.Y);

     if (abs(m.end.X - m.start.X) > 1 || abs(m.end.Y - m.start.Y) > 1)
     {    
          sprintf(message, "%s", "Invalid move: King can only move one square in any direction\n");
          // printf("%s", "Invalid move: King can only move one square in any direction\n");
          return false;
     }

     if (endPiece.type != EMPTY && endPiece.isWhite == startPiece.isWhite)
     {
          sprintf(message, "%s", "Invalid move: Can't take same team's piece\n");
          // printf("%s", "Invalid move: Can't take same team's piece\n");
          return false;
     }

     return true;
}
bool checkValidMoveQueen(const Board *chessBoard, Move m, Message message)
{
     // For readability
     Piece startPiece = getPieceAt(chessBoard, m.start.X, m.start.Y);
     Piece endPiece = getPieceAt(chessBoard, m.end.X, m.end.Y);

     // Check move type(horizontal, vertical, or diagonal)
     if (m.start.X != m.end.X && m.start.Y != m.end.Y && abs(m.end.X - m.start.X) != abs(m.end.Y - m.start.Y))
     {
          sprintf(message, "%s", "Invalid move: Queen must move in a straight line or diagonally\n");
          return false;
     }

     // Determine direction
     int xDirection = (m.end.X > m.start.X) ? 1 : (m.end.X < m.start.X) ? -1
                                                            : 0;
     int yDirection = (m.end.Y > m.start.Y) ? 1 : (m.end.Y < m.start.Y) ? -1
                                                            : 0;

     // Check for obstacles along the path
     int x = m.start.X + xDirection;
     int y = m.start.Y + yDirection;
     while (x != m.end.X || y != m.end.Y)
     {
          if (getPieceAt(chessBoard, x, y).type != EMPTY)
          {
               sprintf(message, "%s", "Invalid move: There is a piece in the way\n");
               return false;
          }
          x += xDirection;
          y += yDirection;
     }

     // Check end position for same-team piece
     if (endPiece.type != EMPTY && endPiece.isWhite == startPiece.isWhite)
     {
          sprintf(message, "%s", "Invalid move: Can't take same team's piece\n");
          return false;
     }

     return true;
}

bool checkValidMove(const Board *chessBoard, Move move, Message message)
{
     if(message == NULL) {
          sprintf(message, "%s", "Error: Invalid message buffer supplied as argument\n");
     }

     message[0] = '\0';
     
     if (chessBoard == NULL)
     {
          sprintf(message, "%s", "Error: Invalid chess board.\n");
          return false;
     }

     if (!checkValidPos(move.start) ||
         !checkValidPos(move.end))
     {
          sprintf(message, "%s", "Error: Move out of bounds.\n");
          return false;
     }

     if(move.start.X == move.end.X && move.start.Y == move.end.Y) {
          sprintf(message, "%s", "Error: Start and end move is the same.\n");
          return false;
     }

     Piece startPiece = getPieceAt(chessBoard, move.start.X, move.start.Y);
     switch (startPiece.type)
     {
     case (PAWN):
          return checkValidMovePawn(chessBoard, move, message);

     case (ROOK):
          return checkValidMoveRook(chessBoard, move, message);

     case (KNIGHT):
          return checkValidMoveKnight(chessBoard, move, message);

     case (BISHOP):
          return checkValidMoveBishop(chessBoard, move, message);

     case (KING):
          return checkValidMoveKing(chessBoard, move, message);

     case (QUEEN):
          return checkValidMoveQueen(chessBoard, move, message);

     default:
          sprintf(message, "%s", "Invalid move: No piece at start location\n");
          return false;
     }
}
