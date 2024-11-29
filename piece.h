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

typedef enum specialCases {
     CASE_ERROR = -1, CASE_NONE, CASE_CASTLE_KINGSIDE, CASE_CASTLE_QUEENSIDE, CASE_EN_PASSANT, 
     CASE_PROMOTION_QUEEN, CASE_PROMOTION_ROOK, CASE_PROMOTION_BISHOP, CASE_PROMOTION_KNIGHT 
} SpecialCase;

typedef struct {
     Pos start; 
     Pos end;
     SpecialCase specialCase;
} Move;

Move createMove(int startX, int startY, int endX, int endY, SpecialCase specialCase);

typedef struct {
     int startFile;
     int startRank;
     int endFile;
     int endRank;
     PieceType type;
     SpecialCase specialCase; 
} Prompt;

Prompt createPrompt(int startfile, int startRank, int endFile, int endRank, PieceType type, SpecialCase specialCase);

typedef enum {
     CASTLE_KINGSIDE,
     CASTLE_QUEENSIDE
} CastleType;

typedef struct {
     Piece boardArray[8][8];
     Color currentTurn;
     bool isGameEnded;
     bool isWhiteChecked;
     bool isBlackChecked;
     bool castlingImpossible[2][2];
} Board;


Piece getPieceAt(const Board* board, int file, int rank);

typedef char Message[100];

#endif // PIECE_H
