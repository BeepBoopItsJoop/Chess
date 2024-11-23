#include <stdio.h>
#include <string.h>
#include <regex.h>

#include "piece.h"

// // Function to determine the piece based on the input character
enum pieces get_piece_type(char piece_char)
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

Prompt promptMove()
{
     const int INPUT_SIZE = 20;
     const char *regex_pattern = "^(O-O-O|O-O|[KQRBN]?[a-h]?[1-8])$";
     // const char *regex_pattern = "^(O-O-O|O-O|[KQRBN]?[a-h]?[1-8]?[x]?[a-h][1-8](=[QRBN])?[+#]?)$";
     //             better patten    ^([NBRQK])?([a-h])?([1-8])?(x)?([a-h][1-8])(=[NBRQK])?(\+|#)?$|^O-O(-O)?$/

     char input[INPUT_SIZE];
     regex_t regex;
     int reti;

     reti = regcomp(&regex, regex_pattern, REG_EXTENDED);
     if (reti)
     {
          fprintf(stderr, "Could not compile regex\n");
          regfree(&regex);
          return (Prompt){-1, -1, -1, CASE_NONE};
     }

     while (1)
     {
          printf("Enter your move in algebraic notation: ");
          fgets(input, sizeof(input), stdin);

          // Remove newline from the input if present
          input[strcspn(input, "\n")] = 0;
 
          regmatch_t match;
          reti = regexec(&regex, input, 1, &match, 0);
          // Regex matched, proceed to parse the input
          if (!reti)
          {
               if (strncmp(input, "O-O", INPUT_SIZE) == 0)
               {
                    // TODO: Implement castling
                    break;
               }
               else if (strncmp(input, "O-O-O", INPUT_SIZE) == 0)
               {
                    // TODO: ^
                    break;
               }

               char piece_char = input[0];
               enum pieces piece_type;
               // Determine if the move has a specific piece type (K, Q, R, B, N) or is a pawn move
               if (piece_char == 'K' || piece_char == 'Q' || piece_char == 'R' ||
                   piece_char == 'B' || piece_char == 'N')
               {
                    piece_type = get_piece_type(piece_char);
               }
               else
               {
                    piece_type = PAWN;
               }

               char file = input[match.rm_eo - 2];
               char rank = input[match.rm_eo - 1];

               // TODO: function for creating a prompt
               return (Prompt){file - 'a' + 1, rank - '0', piece_type};

               // TODO: implement optional en passant in prompt 
               // TODO: implement optional take(x) in prompt 
               // TODO: implement optional destination piece in prompt 
          }
          else if (reti == REG_NOMATCH)
          {
               printf("%s", "Invalid move format. Please try again.\n");
          }
          else
          {
               regerror(reti, &regex, input, sizeof(input));
               fprintf(stderr, "Regex match failed: %s\n", input);
               regfree(&regex);
               return (Prompt){-1, -1, -1, CASE_NONE};
          }
     }

     // Free memory allocated to the pattern buffer by regcomp()
     regfree(&regex);
}
