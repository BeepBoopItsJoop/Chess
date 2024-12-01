#include "piece.h"
#include <stdio.h>

size_t getPieceCount(PieceType type) {
     const size_t pieceCount[] = {0, PAWN_COUNT, ROOK_COUNT, KNIGHT_COUNT, BISHOP_COUNT, QUEEN_COUNT, KING_COUNT};
     if (type < EMPTY || type > KING) return 0;
     return (pieceCount[type]);
}

Move createMove(int startX, int startY, int endX, int endY, SpecialCase specialCase) {
     return (Move){(Pos){startX, startY}, (Pos){endX, endY}, specialCase};
}

Prompt createPrompt(int startfile, int startRank, int endFile, int endRank, PieceType type, SpecialCase specialCase) {
     return (Prompt){startfile, startRank, endFile, endRank, type, specialCase};
}

bool checkValidPos(Pos pos) {
     if (pos.X < 1 || pos.X > 9 || pos.Y < 1 || pos.Y > 9)
     {
          // fprintf(stderr, "Error: Invalid out-of-bouns position supplied as argument.\n");
          return false;
     }
     return true;
}

Piece getPieceAt(const Board* board, int file, int rank) {
     if (board == NULL)
     {
          fprintf(stderr, "Error: Invalid chess board.\n");
          return (Piece){-1, 0};
     }

     return board->boardArray[rank-1][file-1];
}
