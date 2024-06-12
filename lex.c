/*
COP3402 Programming Project HW2
Trever Jones
Jose Porta
*/

#include <stdio.h>
#include <stdlib.h>

// Max length macros (Jose Porta)
#define MAX_ID_LEN 11
#define MAX_NUM_LEN 5

// Token type enum (Jose Porta)
typedef enum {
  skipsym = 1,
  identsym,
  numbersym,
  plussym,
  minussym,
  multsym,
  slashsym,
  fisym,
  eqsym,
  neqsym,
  lessym,
  leqsym,
  gtrsym,
  geqsym,
  lparentsym,
  rparentsym,
  commasym,
  semicolonsym,
  periodsym,
  becomessym,
  beginsym,
  endsym,
  ifsym,
  thensym,
  whilesym,
  dosym,
  callsym,
  constsym,
  varsym,
  procsym,
  writesym,
  readsym,
  elsesym
} token_type;

// Reserved words (Jose Porta)
char *reserved[] = {"const", "var", "procedure", "call", "begin",
                    "end",   "if",  "fi",        "then", "else",
                    "while", "do",  "read",      "write"};

// Token struct (Jose Porta)
typedef struct Token {
  token_type type;
  char lexeme[MAX_ID_LEN + 1];
} Token;

// Main parsing loop (Jose Porta)
void parser(long f_sz, char input_arr[]) {
  int comment_state = 0;
  int parser_pos = 0;
  for (int i = 0; i < f_sz; i++) {
    char next_char = input_arr[i];
    // Detect comments
    switch (next_char) {
    case '/':
      if (comment_state == 0) {
        // Comment state partially active
        comment_state = 1;
        parser_pos = i;
      } else if (comment_state == 1) {
        // Comment state deactivated
        comment_state = 0;
        continue;
      }
      break;
    case '*':
      if (comment_state == 1) {
        // Comment state active
        comment_state = 2;
      } else if (comment_state == 2) {
        // Comment state partially deactivated
        comment_state = 1;
      }

      break;
    default:
      break;
    }

    if (comment_state > 0) {
      continue;
    }

    printf("%c", next_char);
  }
}

int main(int argc, char *argv[]) {
  char *inputArr = NULL;
  FILE *file;
  file = fopen(argv[1], "r");
  long fileSize = 0;

  // Checking for correct number of argumnets passed (Trever Jones)
  if (argc != 2) {
    printf("Please use correct command: %s <inputFileName>\n", argv[0]);
    return 1;
  }

  // Reading in input from input file (Trever Jones)
  if (file != NULL) {

    // Get the size of the file (Trever Jones)
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Dynamically allocate input array (Trever Jones)
    inputArr = (char *)malloc(fileSize + 1);
    if (inputArr == NULL) {
      printf("Memory allocation failed\n");
      fclose(file);
      return 1;
    }

    // Read in file contents (Trever Jones)
    fread(inputArr, 1, fileSize, file);
    inputArr[fileSize] = '\0';
    fclose(file);
  }

  // File name passed incorrectly as argumnent or file does not exsist (Trever
  // Jones)
  else {
    printf("No file named %s found\n", argv[1]);
    return 1;
  }

  printf("Source Program:\n%s\n", inputArr);
  parser(fileSize, inputArr);
  free(inputArr);
  return 0;
}