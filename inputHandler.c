#include <stdio.h>
#include <string.h>
#include <regex.h>

#include "piece.h"

// // Function to determine the piece based on the input character
PieceType get_piece_type(char piece_char)
PieceType get_piece_type(char piece_char)
{
     switch (piece_char)
     {
     case 'K':
          return KING;
     case 'Q':
          return QUEEN;
     case 'R':
          return ROOK;
     case 'B':
          return BISHOP;
     case 'N':
          return KNIGHT;
     default:
          return PAWN; // If no character, assume it's a pawn move
     }
}

const int INPUT_SIZE = 20;

Prompt parsePrompt(const char* input, const regmatch_t match) {
     // Notation as follows:
     // e3
     // Ne3
     // exd5
     // Nxd5
     // exd6 e.p.
     // e5xd6 e.p.
     // e8=Q
     // d5xd6=Q
     // exd6=R
     // Qe5+
     // Nf6+
     // Bf4+
     // Qh5#
     // Nf7#
     // Qf6#
     // *Piece* *startFile* *startRank* *x* *endFile* *endRank* *# or + or e.p."
     // TODO: test edge cases

     int startRank = -1, startFile = -1;  
     int endRank = -1, endFile = -1;  
     PieceType type = PAWN;
     SpecialCase specialCase = CASE_NONE;

     if (strncmp(input, "O-O", INPUT_SIZE) == 0)
     {
          return (Prompt){-1, -1, -1, -1, EMPTY, CASE_CASTLE_KINGSIDE};
     }
     else if (strncmp(input, "O-O-O", INPUT_SIZE) == 0)
     {
          return (Prompt){-1, -1, -1, -1, EMPTY, CASE_CASTLE_QUEENSIDE};
     }

     if(input[0] == 'K' || input[0] == 'Q' || input[0] == 'R' || input[0] == 'B' || input[0] == 'N') {
          type = get_piece_type(input[0]);
          input++;
     }

     if (input[0] >= 'a' && input[0] <= 'h') {
          startFile = input[0] - 'a' + 1;
          input++; // Skip the rank character
     }

     if (input[0] >= '1' && input[0] <= '8') {
          startRank = input[0] - '0';
          input++; // Skip the rank character
     }

     if(input[0] == 'x') {
          input++;
     }

     if (input[0] >= 'a' && input[0] <= 'h') {
          endFile = input[0] - 'a' + 1;
          input++; // Skip the rank character
     }


     if (input[0] >= '1' && input[0] <= '8') {
          endRank = input[0] - '0';
          input++; // Skip the rank character
     }

     if(input[0] == '=') {
          input++;
          if(input[0] == 'Q' || input[0] == 'R' || input[0] == 'B' || input[0] == 'N') {
               PieceType promotionType = get_piece_type(input[0]);
               switch (promotionType)
               {
               case QUEEN:
                    specialCase = CASE_PROMOTION_QUEEN;
                    break;
               case ROOK:
                    specialCase = CASE_PROMOTION_ROOK;
                    break;
               case BISHOP:
                    specialCase = CASE_PROMOTION_BISHOP;
                    break;
               case KNIGHT:
                    specialCase = CASE_PROMOTION_KNIGHT;
                    break;
               }
               input++;
          }
     } 

     if(endFile == -1) {    
          endFile = startFile;
          startFile = -1;
     }
     if(endRank == -1) {
          endRank = startRank;
          startRank = -1;
     }
     
     return createPrompt(startFile, startRank, endFile, endRank, type, specialCase);
}

Prompt promptMove()
{
     // const char *regex_pattern = "^(O-O-O|O-O|[KQRBN]?[a-h]?[1-8])$";
     // const char *regex_pattern = "^(O-O-O|O-O|[KQRBN]?[a-h]?[1-8]?[x]?[a-h][1-8](=[QRBN])?[+#]?)$";
     const char *regex_pattern = "^([NBRQK])?([a-h])?([1-8])?(x)?([a-h][1-8])(e\\.p\\.)?(=[NBRQ])?(\\+|#)?$|^O-O(-O)?$";

     char input[INPUT_SIZE];
     regex_t regex;
     int reti;

     reti = regcomp(&regex, regex_pattern, REG_EXTENDED);
     if (reti)
     {
          fprintf(stderr, "Could not compile regex\n");
          regfree(&regex);
          return (Prompt){-1, -1, -1, -1, EMPTY, CASE_ERROR};
     }

     Prompt prompt;
     while (1)
     {
          printf("Enter your move in algebraic notation: ");
          fgets(input, sizeof(input), stdin);

          // Remove newline from the input if present
          input[strcspn(input, "\n")] = 0;
 
          regmatch_t match;
          reti = regexec(&regex, input, 1, &match, 0);
          if (!reti)
          {
               prompt = parsePrompt(input, match);
               break;
          }
          else if (reti == REG_NOMATCH)
          {
               printf("%s", "Invalid move format. Please try again.\n");
          }
          else
          {
               regerror(reti, &regex, input, sizeof(input));
               fprintf(stderr, "Regex match failed: %s\n", input);
               prompt = (Prompt){-1, -1, -1, -1, EMPTY, CASE_ERROR};
               break;
          }
     }

     // Free memory allocated to the pattern buffer by regcomp()
     regfree(&regex);

     return prompt;
}
