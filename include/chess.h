#pragma once

#include <stdbool.h>
#include <stddef.h> // size_t
#include <string>

enum class PieceType {
     empty,
     pawn,
     rook,
     knight,
     bishop,
     queen,
     king
};

enum PieceCount {
     none = 0,
     pawn_count = 8,
     rook_count = 2,
     kngiht_count = 2,
     bishop_count = 2,
     queen_count = 1,
     king_count = 1
};

enum class CastleType{
     kingside,
     queenside
};

enum class SpecialCase{
     error = -1,
     none = 0,
     castle_kingside,
     castle_queenside,
     en_passant,
     promotion_queen,
     promotion_rook,
     promotion_bishop,
     promotion_knight,
};

struct Prompt{
     Prompt() {};
     Prompt(int startfile, int startRank, int endFile, int endRank, PieceType type, SpecialCase specialCase) 
     : startFile(startFile), startRank(startRank), endFile(endFile), endRank(endRank), type(type), specialCase(specialCase) {} ;

     int startFile;
     int startRank;
     int endFile;
     int endRank;
     PieceType type;
     SpecialCase specialCase; 
};


struct Pos {
     Pos() : X{-1}, Y{-1} {};
     Pos(int X, int Y) : X{X}, Y{Y} {};
     bool checkValid();

     int X;
     int Y;
};

struct Move {
     Move(int startX, int startY, int endX, int endY, SpecialCase specialCase) 
     : start{startX, startY}, end{endX, endY}, specialCase(specialCase) {};

     Pos start;
     Pos end;
     SpecialCase specialCase;
};



size_t getPieceCount(PieceType type);

enum class Color {
     black,
     white
};

struct Piece {
     Piece() : type(PieceType::empty), color(Color::white) {}
     Piece(PieceType type, Color color) : type(type), color(color) {}

     PieceType type;
     Color color;
};


class Chess {
    public:
     Chess(/* args */);

     void Run();

     bool checkValidMove(Move move);

     Piece getPieceAt(int file, int rank);
     size_t getPieceCount(PieceType type);

     Piece boardArray[8][8]{};
     Color currentTurn{};
     bool isGameEnded{};
     bool isWhiteChecked{};
     bool isBlackChecked{};
     bool castlingImpossible[2][2]{};

    private:
     std::string message{};

     void gameRound(Move move);

     bool isSquareContested(Pos square);
     bool legalMoveExists();
     
     Move getInputMove();
     bool playMove(Move move);
     bool checkCastleConditions(Prompt input);
     void setCastleRequirements(Move move) ;
     
     
     void performCastle(Move move);
     Pos findNthPiece(PieceType type, int n);
     void movePiece(Move move);


     bool checkValidMovePawn(Move m);
     bool checkValidMoveRook(Move m);
     bool checkValidMoveKnight(Move m);
     bool checkValidMoveBishop(Move m);
     bool checkValidMoveKing(Move m);
     bool checkValidMoveQueen(Move m);
};
