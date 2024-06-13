/*
COP3402 Programming Project HW2
Trever Jones
Jose Porta
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Max length macros (Jose Porta)
#define MAX_ID_LEN 11
#define MAX_NUM_LEN 5
#define TOKEN_TBL_SZ 1000

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
  elsesym,
  colonsym
} token_type;

// Reserved words (Jose Porta)
char *reserved[] = {"const", "var", "procedure", "call", "begin",
                    "end",   "if",  "fi",        "then", "else",
                    "while", "do",  "read",      "write"};
int res_enums[] = {constsym, varsym, procsym, callsym, beginsym,
                   endsym,   ifsym,  fisym,   thensym, elsesym,
                   whilesym, dosym,  readsym, writesym};

// Token struct (Jose Porta)
typedef struct Token {
  token_type type;
  char lexeme[MAX_ID_LEN + 1];
} Token;
Token curr_token = {0, ""};
// Token Table
Token tokenList[TOKEN_TBL_SZ];
int num_tokens = 0;

// Determine Special Symbol type
int is_sym(char c) {
  switch (c) {
  case '+':
    return plussym;

  case '-':
    return minussym;

  case '*':
    return multsym;

  case '/':
    return slashsym;

  case '(':
    return lparentsym;

  case ')':
    return rparentsym;

  case '=':
    return eqsym;

  case ',':
    return commasym;

  case '.':
    return periodsym;

  case '<':
    return lessym;

  case '>':
    return gtrsym;

  case ';':
    return semicolonsym;

  case ':':
    return colonsym;

  default:
    return 0;
  }
}

// Checks if indentifier is a reserved word and if so, returns its type
int is_reserved(char identifier[]) {
  for (int i = 0; i < 14; i++) {
    if (strcmp(reserved[i], identifier) == 0) {
      return res_enums[i];
    }
  }
  return identsym;
}

// Push new token to token table and reset curr_token (Jose Porta)
int tokenize() {
  tokenList[num_tokens] = curr_token;
  printf("Token added: %s\n", tokenList[num_tokens].lexeme);
  num_tokens++;
  curr_token.type = 0;
  for (int i = 0; i < MAX_ID_LEN; i++) {
    curr_token.lexeme[i] = '\0';
  }
  return 0;
}

// Main parsing loop (Jose Porta)
void parser(long f_sz, char input_arr[]) {
  int comment_state = 0;
  int parser_pos = 0;
  int currType = 0;
  int state = 0;

  // i < f_sz prints extra chars and f_sz - 10 fixes it for some reason? (delete
  // before submission)
  for (int i = 0; i < f_sz - 10; i++) {
    char currChar = input_arr[i];
    char nextChar = input_arr[i + 1];
    // Detect comments
    switch (currChar) {
    case '/':
      if (nextChar == '*') {
        // Comment state active
        comment_state = 1;
        parser_pos = i;
        continue;
      }
      break;
    case '*':
      if (nextChar == '/' && comment_state == 1) {
        // Comment state dectivated
        comment_state = 0;
        i += 1;
        continue;
      }

    default:
      if (comment_state == 1) {
        continue;
      }
      break;
    }
    // printf("%c",input_arr[i]);

    switch (state) {
    case 0:
      // If currChar is a letter -> state = identifier
      if (isalpha(currChar)) {
        // switch FSA to identity state
        state = identsym;
        // Set token type
        curr_token.type = state;
        // "rewind" loop to process char in as part of identifier
        i--;
        break;
      }
      // If currChar is a digit -> state = number
      if (isdigit(currChar)) {
        // Switch to number state
        state = numbersym;
        // Set current token to numbersym
        curr_token.type = state;
        // "rewind" loop to process char in as part of identifier
        i--;
        break;
      }

      // if currChar is a special character (Trever Jones)
      if (is_sym(currChar) != 0) {
        // Switch to state based on specific symbol type
        state = is_sym(currChar);
        curr_token.type = state;
        // "rewind" loop to process char in as symbol
        i--;
        break;
      }

      // If currChar is a space, skip to next char (Trever Jones)
      if (isspace(currChar)) {
        state = 0;
        break;
      }

      // Handle invalid symbol (Trever Jones)
      else {
        state = 0;
        printf("Invalid symbol\n");
      }
      break;

    case identsym:
      // identifiers have the form: letter(letter | digit)*
      // Add char to identifier name
      curr_token.lexeme[strlen(curr_token.lexeme)] = currChar;

      if (isalpha(currChar) || isdigit(currChar)) {
        // Check if identifier is a reserved word (returns identsym if not)
        state = is_reserved(curr_token.lexeme);
      }

      // Non alpha-numeric value implies end of identifier
      if (!isalpha(nextChar) && !isdigit(nextChar)) {
        state = tokenize();
      }
      if (strlen(curr_token.lexeme) > MAX_ID_LEN) {
        printf("Error: identifier %s execeeds max length (11)",
               curr_token.lexeme);
      }
      break;

    case numbersym:
      curr_token.lexeme[strlen(curr_token.lexeme)] = currChar;

      // NOTE: how to handle numbers >5 digits?
      if (strlen(curr_token.lexeme) > MAX_NUM_LEN) {
        printf("Error: number %s execeeds max length (5)", curr_token.lexeme);
      }
      if (!isdigit(nextChar)) {
        tokenize();
        state = 0;
      }

      break;
    case plussym:
    case minussym:
    case multsym:
    case slashsym:
    case rparentsym:
    case lparentsym:
    case eqsym:
    case commasym:
    case periodsym:
    case lessym:
    case gtrsym:
    case semicolonsym:
    case colonsym:
      curr_token.lexeme[strlen(curr_token.lexeme)] = currChar;
      tokenize();
      state = 0;
      break;
    case constsym:
    case varsym:
    case procsym:
    case callsym:
    case beginsym:
    case endsym:
    case ifsym:
    case fisym:
    case thensym:
    case elsesym:
    case whilesym:
    case dosym:
    case readsym:
    case writesym:
      if (isalpha(nextChar) || isdigit(nextChar)) {
        // Extra alpha-numeric characters imply identifier isn't a reserved word
        // e.g. 'var' is reserved but 'vars55' is a valid, non-reserved
        // identifier
        state = identsym;
        // rewind loop to process current char as part of identsym
        i--;
        break;
      }
      // Non alpha-numeric value implies end of identifier
      if (!isalpha(nextChar) && !isdigit(nextChar)) {
        state = tokenize();
        i--;
        break;
      }
    default:
      break;
    }
    printf("State: %d, lexeme: %s\n", state, curr_token.lexeme);
    printf("%s\n", curr_token.lexeme);
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

  printf("Source Program:\n%s\n\n", inputArr);
  printf("Lexeme Table:\n\n");

  // Parse inputArr
  parser(fileSize, inputArr);
  free(inputArr);

  printf("Token Table:\n");
  for (int i = 0; i < num_tokens; i++) {
    printf("%s ", tokenList[i].lexeme);
  }

  return 0;
}