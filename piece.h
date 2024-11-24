#ifndef PIECE_H
#define PIECE_H

#include <stdbool.h>
#include <stddef.h> // size_t

typedef enum pieces {
     EMPTY, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING
} PieceType;

enum PieceCount {
     PAWN_COUNT = 8, ROOK_COUNT = 2, KNIGHT_COUNT = 2, BISHOP_COUNT = 2, QUEEN_COUNT = 1, KING_COUNT = 1
};

size_t getPieceCount(PieceType type);

typedef enum color {
     BLACK, WHITE   
} Color; 

typedef struct {
     int type;
     bool isWhite;
} Piece;

typedef struct {
     int X;
     int Y;
} Pos;

bool checkValidPos(Pos pos);

typedef struct {
     Pos start; 
     Pos end; 
} Move;

Move createMove(int startX, int startY, int endX, int endY);

typedef enum specialCases {
     CASE_ERROR, CASE_NONE, CASE_EN_PASSANT, CASE_CASTLE_KINGSIDE, CASE_CASTLE_QUEENSIDE
} SpecialCase;

typedef struct {
     int file;
     int rank;
     PieceType type;
     enum specialCases specialCase; 
} Prompt;

typedef struct {
     Piece boardArray[8][8];
     Color currentTurn;
     bool isGameEnded;
     bool isWhiteChecked;
     bool isBlackChecked;
} Board;

Piece getPieceAt(const Board* board, int file, int rank);

typedef char Message[100];

#endif // PIECE_H
