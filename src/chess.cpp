#include "chess.h"
#include "inputHandler.h"
#include "displayer.h"

#include <stdlib.h> // for abs
#include <iostream>
#include <sstream>


bool Pos::checkValid() {
     if (X < 1 || X > 9 || Y < 1 || Y > 9) {
          // fprintf(stderr, "Error: Invalid out-of-bouns position supplied as argument.\n");
          return false;
     }
     return true;
}

Piece Chess::getPieceAt(int file, int rank) {
     return boardArray[rank - 1][file - 1];
}

size_t Chess::getPieceCount(PieceType type) {
     const PieceCount pieceCount[] = {none, pawn_count, rook_count, kngiht_count, bishop_count, queen_count, king_count};
     if (type < PieceType::empty || type > PieceType::king) return 0;
     return (pieceCount[static_cast<size_t>(type)]);
}


// TODO: modify to include disambiguation using start file and rank
bool Chess::checkValidMove(Move move)
{
     if (! move.start.checkValid()||
         !move.end.checkValid())
     {
          message = "Error: Move out of bounds.\n";
          return false;
     }

     if(move.start.X == move.end.X && move.start.Y == move.end.Y) {
          message = "Error: Start and end move is the same.\n";
          return false;
     }

     Piece startPiece = getPieceAt(move.start.X, move.start.Y);
     switch (startPiece.type)
     {
     case (PieceType::pawn):
          return checkValidMovePawn(move);

     case (PieceType::rook):
          return checkValidMoveRook(move);

     case (PieceType::knight):
          return checkValidMoveKnight(move);

     case (PieceType::bishop):
          return checkValidMoveBishop(move);

     case (PieceType::king):
          return checkValidMoveKing(move);

     case (PieceType::queen):
          return checkValidMoveQueen(move);

     default:
          message = "Invalid move: No piece at start location\n";
          return false;
     }
}

bool Chess::checkValidMovePawn(Move m)
{
     // For readability
     Piece startPiece = getPieceAt(m.start.X, m.start.Y);
     Piece endPiece = getPieceAt(m.end.X, m.end.Y);

     // Account which way is forward (black or white)
     int forward = ((startPiece.color == Color::white) ? 1 : -1);

     // If Forward 1 move
     if ((m.end.X == m.start.X) && (m.end.Y - m.start.Y == (1 * forward)))
     {
          // Check if piece is there
          if (endPiece.type != PieceType::empty)
          {
               message = "Invalid move: Piece in the way\n";
               return false;
          }
          return true;
     }

     // If Forward 2 moves
     if ((m.end.X == m.start.X) && (m.end.Y - m.start.Y == (2 * forward)))
     {
          // Check if starting position
          if (!(startPiece.color == Color::white && m.start.Y == 2 
               || !(startPiece.color == Color::white) && m.start.Y == 7))
          {
               message = "Invalid move: Pawn must be in starting location to move 2 squares forward\n";
               return false;
          }
          // check if piece is directly infront
          if (getPieceAt(m.end.X, m.start.Y + (1 * forward)).type != PieceType::empty)
          {
               message = "Invalid move: Piece directly infront\n";
               return false;
          }
          // Check if piece is there
          if (endPiece.type != PieceType::empty)
          {
               message = "Invalid move: Piece already there\n";
               return false;
          }
          return true;
     }

     // If 1 column to either side
     if ((abs(m.end.X - m.start.X) == 1) && (abs(m.end.Y - m.start.Y) == 1))
     {
          // Check if there is a piece there
          if (endPiece.type == PieceType::empty)
          {
               message = "Invalid move: There is no piece to take\n";
               return false;
          }
          // Check if piece is same team
          if (endPiece.color == startPiece.color)
          {
               message = "Invalid move: Piece of same colour is there\n";
               return false;
          }
          return true;
     }

     // Else invalid
     message = "Invalid move: Pawn can't move that way\n";
     return false;
}
bool Chess::checkValidMoveRook(Move m)
{
     // For readability
     Piece startPiece = getPieceAt(m.start.X, m.start.Y);
     Piece endPiece = getPieceAt(m.end.X, m.end.Y);

     if (m.start.X != m.end.X && m.start.Y != m.end.Y)
     {
          message = "Invalid move: Rook can't move diagonally\n";
          return false;
     }

     if (m.start.Y == m.end.Y)
     {
          int direction = (m.start.X < m.end.X) ? 1 : -1;
          // i = Startx + direction; - To not check the starting location
          // i != endX; - Check every square except the last one
          for (size_t x = m.start.X + direction; x != m.end.X; x += direction)
          {
               
               if (getPieceAt(x, m.start.Y).type != PieceType::empty)
               {
                    message = "Invalid move: There is a piece in the way\n";
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
               if (getPieceAt(m.start.X, y).type != PieceType::empty)
               {
                    message = "Invalid move: There is a piece in the way\n";
                    return false;
               }
          }
     }

     // Check if there is a piece at the end that can be taken
     if (endPiece.type != PieceType::empty && endPiece.color == startPiece.color)
     {
          message = "Invalid move: Can't take same team's piece\n";
          return false;
     }

     return true;
}
bool Chess::checkValidMoveKnight(Move m)
{
     // For readability
     Piece startPiece = getPieceAt(m.start.X, m.start.Y);
     Piece endPiece = getPieceAt(m.end.X, m.end.Y);

     // -+2x && -+1y || -=1x -+2y
     if (!(
             (abs(m.end.X - m.start.X) == 2 && abs(m.end.Y - m.start.Y) == 1) ||
             (abs(m.end.X - m.start.X) == 1 && abs(m.end.Y - m.start.Y) == 2)))
     {
          message = "Invalid move: Knight can't move that way\n";
          return false;
     }

     // Check if there is a piece at the end that can be taken
     if (endPiece.type != PieceType::empty && endPiece.color == startPiece.color)
     {
          message = "Invalid move: Can't take same team's piece\n";
          return false;
     }

     return true;
}
bool Chess::checkValidMoveBishop(Move m)
{
     // For readability
     Piece startPiece = getPieceAt(m.start.X, m.start.Y);
     Piece endPiece = getPieceAt(m.end.X, m.end.Y);

     if (abs(m.end.X - m.start.X) != abs(m.end.Y - m.start.Y))
     {
          message = "Invalid move: Bishop must move diagonally\n";
          return false;
     }

     int xDirection = (m.end.X > m.start.X) ? 1 : -1;
     int yDirection = (m.end.Y > m.start.Y) ? 1 : -1;

     int x = m.start.X + xDirection;
     int y = m.start.Y + yDirection;
     while (x != m.end.X && y != m.end.Y)
     {
          if (getPieceAt(x, y).type != PieceType::empty)
          {
               
               message = "Invalid move: There is a piece in the way\n";
               return false;
          }
          x += xDirection;
          y += yDirection;
     }

     if (endPiece.type != PieceType::empty && endPiece.color == startPiece.color)
     {
          message = "Invalid move: Can't take same team's piece\n";
          return false;
     }

     return true;
}
bool Chess::checkValidMoveKing(Move m)
{
     // For readability
     Piece startPiece = getPieceAt(m.start.X, m.start.Y);
     Piece endPiece = getPieceAt(m.end.X, m.end.Y);

     if (abs(m.end.X - m.start.X) > 1 || abs(m.end.Y - m.start.Y) > 1)
     {    
          message = "Invalid move: King can only move one square in any direction\n";
          return false;
     }

     if (endPiece.type != PieceType::empty && endPiece.color == startPiece.color)
     {
          message = "Invalid move: Can't take same team's piece\n";
          return false;
     }

     return true;
}
bool Chess::checkValidMoveQueen(Move m)
{
     // For readability
     Piece startPiece = getPieceAt(m.start.X, m.start.Y);
     Piece endPiece = getPieceAt(m.end.X, m.end.Y);

     // Check move type(horizontal, vertical, or diagonal)
     if (m.start.X != m.end.X && m.start.Y != m.end.Y && abs(m.end.X - m.start.X) != abs(m.end.Y - m.start.Y))
     {
          message = "Invalid move: Queen must move in a straight line or diagonally\n";
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
          if (getPieceAt(x, y).type != PieceType::empty)
          {
               message = "Invalid move: There is a piece in the way\n";
               return false;
          }
          x += xDirection;
          y += yDirection;
     }

     // Check end position for same-team piece
     if (endPiece.type != PieceType::empty && endPiece.color == startPiece.color)
     {
          message = "Invalid move: Can't take same team's piece\n";
          return false;
     }

     return true;
}

Chess::Chess() {
     using enum PieceType;
     PieceType pieceArrangement[] = {rook, knight, bishop, queen, king, bishop, knight, rook};

     currentTurn = Color::white;
     isGameEnded = false;

     // Sets starting pieces
     // For white
     for (size_t i = 0; i < 8; i++)
          boardArray[0][i] = Piece{pieceArrangement[i], Color::white};
     for (size_t i = 0; i < 8; i++)
          boardArray[1][i] = Piece{PieceType::pawn, Color::white};

     // For black
     for (size_t i = 0; i < 8; i++)
          boardArray[7][i] = Piece{pieceArrangement[i], Color::black};
     for (size_t i = 0; i < 8; i++)
          boardArray[6][i] = Piece{PieceType::pawn, Color::black};
}

void Chess::Run() {
     
     boardPrint(*this);
     while (!isGameEnded) {
          std::cout << "It is " << ((currentTurn == Color::white) ? "White" : "Black") << "'s turn\n";
          gameRound(getInputMove());
     }

     printf("%s", "end\n");
}

void Chess::gameRound(Move move) {
     if (move.specialCase == SpecialCase::castle_kingside || move.specialCase == SpecialCase::castle_queenside) {
          performCastle(move);
     } else if (!playMove(move)) {
          // fprintf(stderr, "Error: Invalid move supplied\n");
          return;
     }

     setCastleRequirements(move);

     boardPrint(*this);

     currentTurn = (currentTurn == Color::black) ? Color::white : Color::black;

     // Check for mate
     Pos currTurnKing = findNthPiece(PieceType::king, 1);
     if (isSquareContested(currTurnKing)) {
          if (currentTurn == Color::white)
               isWhiteChecked = true;
          else
               isBlackChecked = true;
     } else {
          isBlackChecked = isWhiteChecked = false;
     }

     // Check for checkmate or stalemate
     if (!legalMoveExists()) {
          if ((currentTurn == Color::black && isBlackChecked) || isWhiteChecked) {
               // Check Mate
               isGameEnded = true;
          } else {
               // TODO: implement stalemate
               isGameEnded = true;
          }
     }
}

bool Chess::isSquareContested(Pos square) {
     for (int y = 1; y < 9; y++) {
          for (int x = 1; x < 9; x++) {
               // Piece cannot be under attack by same color
               if (getPieceAt(x, y).color == currentTurn) {
                    continue;
               }
               if (checkValidMove(Move{x, y, square.X, square.Y, SpecialCase::none})) {
                    return true;
               }
          }
     }
     return false;
}

void Chess::performCastle(Move move) {
     const int y = (currentTurn == Color::white) ? 1 : 8;

     if (move.specialCase == SpecialCase::castle_kingside) {
          movePiece(Move{5, y, 7, y, SpecialCase::none});  // Move king from 5 to 7
          movePiece(Move{8, y, 6, y, SpecialCase::none});  // Move rook from 8 to 6
     } else {
          movePiece(Move{5, y, 3, y, SpecialCase::none});  // Move king from 5 to 3
          movePiece(Move{1, y, 4, y, SpecialCase::none});  // Move rook from 1 to 4
     }
     std::cout << "Castled" << ((move.specialCase == SpecialCase::castle_kingside) ? "kingside" : "queenside") << '\n';
}

Pos Chess::findNthPiece(PieceType type, int n) {
     int count = 0;
     for (int y = 1; y < 9; y++) {
          for (int x = 1; x < 9; x++) {
               Piece piece = getPieceAt(x, y);
               if (piece.type == type && piece.color == currentTurn) {
                    if (++count == n) {
                         return Pos{x, y};
                    }
               }
          }
     }
     return Pos{-1, -1};  // Not enough matches found
}

void Chess::movePiece(Move move) {
     if (move.start.X < 1 || move.start.X > 8 || move.start.Y < 1 || move.start.Y > 8 ||
         move.end.X < 1 || move.end.X > 8 || move.end.Y < 1 || move.end.Y > 8) {
          message = "Error: Position out of bounds.\n";
          return;
     }

     Piece *startPiece = &boardArray[move.start.Y - 1][move.start.X - 1];
     Piece *endPiece = &boardArray[move.end.Y - 1][move.end.X - 1];

     if (startPiece->type == PieceType::empty) {
          message = "Error: No piece at the start location\n";
          return;
     }

     std::ostringstream oss;
     if (endPiece->type != PieceType::empty) {
          oss << "Piece " << static_cast<int>(startPiece->type)
              << " moves from (" << move.start.X << ", " << move.start.Y << ")"
              << " and takes " << static_cast<int>(endPiece->type)
              << " at (" << move.end.X << ", " << move.end.Y << ")\n";

          message = oss.str();
     } else {
          oss << "Moved piece " << static_cast<int>(startPiece->type)
               << " from (" << move.start.X << ", " << move.start.Y << ")"
               << " to (" << move.end.X << ", " << move.end.Y << ")\n";

          message = oss.str();
     }

     *endPiece = *startPiece;
     *startPiece = Piece{PieceType::empty, Color::black};
}

bool Chess::legalMoveExists() {
     // Loop through every piece on the board
     for (int i = 1; i < 9; i++)
     {
          for (int j = 1; j < 9; j++)
          {
               // Loop through every potential move for the piece
               for (int k = 1; k < 9; k++)
               {
                    for (int l = 1; l < 9; l++)
                    {
                         Chess tempBoard = *this;
                         // Starting piece must be of the current player and not empty
                         Piece startPiece = tempBoard.getPieceAt(i, j);
                         if (startPiece.color != tempBoard.currentTurn || startPiece.type == PieceType::empty)
                         {
                              continue;
                         }

                         Move move = Move{i, j, k, l, SpecialCase::none};

                         // Check if the move is valid and keeps the king safe
                         if (tempBoard.checkValidMove(move))
                         {
                              if (tempBoard.playMove(move))
                              {
                                   return true; // A legal move exists
                              }
                         }
                    }
               }
          }
     }
     // No legal moves exist
     return false;
}

Move Chess::getInputMove() {
     // Ask for an input that is a valid move
     Prompt input;
     while (true)
     {
          std::cout << message;
          Prompt input = promptMove();

          if (input.specialCase == SpecialCase::castle_kingside || input.specialCase == SpecialCase::castle_queenside)
          {
               if (checkCastleConditions(input))
               {
                    return Move{-1, -1, -1, -1, input.specialCase};
               }
               else
               {
                    message = "Castling invalid\n";
                    continue;
               }
          }

          for (size_t i = 1, count = getPieceCount(input.type); i <= count; i++)
          {
               Pos start = findNthPiece(input.type, i);
               if (checkValidMove(Move{start.X, start.Y, input.endFile, input.endRank, SpecialCase::none}))
               {
                    return Move{start.X, start.Y, input.endFile, input.endRank, SpecialCase::none};
               }
          }
     }
}
bool Chess::playMove(Move move) {
     // TOOD: perform checks here

     // Checks if king is left in check after the move in a temporary board
     Chess tempBoard = *this;

     tempBoard.movePiece(move);

     if (tempBoard.isSquareContested(tempBoard.findNthPiece(PieceType::king, 1)))
     {
          message = "Invalid move: Exposes king to check\n";
          return false;
     }
     else
     {
          movePiece(move);
     }

     return true;
}
bool Chess::checkCastleConditions(Prompt input) {
     CastleType castleType = (input.specialCase == SpecialCase::castle_kingside) ? CastleType::kingside : CastleType::queenside;
     if (castlingImpossible[static_cast<int>(currentTurn)][static_cast<int>(castleType)])
          return false;

     const int kingX = 5;
     const int y = (currentTurn == Color::white) ? 1 : 8;

     const int direction = (input.specialCase == SpecialCase::castle_kingside) ? 1 : -1; // Right for kingside, left for queenside
     const int steps = (input.specialCase == SpecialCase::castle_kingside) ? 2 : 3;      // 2 for kingside, 3 for queenside

     // Check if squares between rook and king are empty
     for (int i = 1; i <= steps; ++i)
     {
          if (getPieceAt(kingX + (i * direction), y).type != PieceType::empty)
               return false;
     }

     // Check if king is in check
     if (isSquareContested(findNthPiece(PieceType::king, 1))) {
          return false;
     }

     // Check if squares between rook and king are contested
     // Chcek if rook is contested
     for (int i = 1; i <= steps + 1; ++i)
     {
          if (isSquareContested(Pos{kingX + i * direction, y}))
               return false;
     }

     return true;
}
void Chess::setCastleRequirements(Move move)  {
     if (!(move.start.Y == 8 || move.start.Y == 1)) return;

     int color = static_cast<int>((move.start.Y == 8) ? Color::black : Color::white);

     if (move.start.X == 1)
          castlingImpossible[color][static_cast<int>(CastleType::queenside)] = true;
     else if (move.start.X == 8)
          castlingImpossible[color][static_cast<int>(CastleType::kingside)] = true;
     else if (move.start.X == 5)
     {
          castlingImpossible[color][static_cast<int>(CastleType::kingside)] = true;
          castlingImpossible[color][static_cast<int>(CastleType::queenside)] = true;
     }
}
