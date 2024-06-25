/*
COP3402 Programming Project HW3
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
typedef enum { skipsym = 1, identsym, numbersym, plussym, minussym, multsym, slashsym, fisym, eqsym,
  neqsym, lessym, leqsym, gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym, periodsym, becomessym,
  beginsym, endsym, ifsym, thensym, whilesym, dosym, callsym, constsym, varsym, procsym, writesym, readsym, elsesym,
} token_type;

// Reserved words (Jose Porta)
char *reserved[] = {"const", "var", "procedure", "call", "begin",
                    "end",   "if",  "fi", "then", "else",
                    "while", "do",  "read", "write"};
int res_enums[] = {constsym, varsym, procsym, callsym, beginsym,
                   endsym,   ifsym,  fisym,   thensym, elsesym,
                   whilesym, dosym,  readsym, writesym};

// Token struct (Jose Porta)
typedef struct Token {
  token_type type;
  char lexeme[100];
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
  return 35;

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

// Helper function to set all 11 chars in lexeme to null char
void reset_lexeme(){
  for (int i = 0; i < MAX_ID_LEN; i++) {
    curr_token.lexeme[i] = '\0';
  }
}

// Push new token to token table and reset curr_token (Jose Porta)
int tokenize() {
  if (curr_token.type == identsym && strlen(curr_token.lexeme) > MAX_ID_LEN) {
    printf("Error: identifier '%s' execeeds max length (11)\n", curr_token.lexeme);
    curr_token.type = 0;
    reset_lexeme();
    return 0;
  }

  else if (curr_token.type == numbersym && strlen(curr_token.lexeme) > MAX_NUM_LEN) {
    printf("Error: number '%s' execeeds max length (5)\n", curr_token.lexeme);
    curr_token.type = 0;
    reset_lexeme();
    return 0;
  }

  tokenList[num_tokens] = curr_token;
  printf("%-15s%d\n", tokenList[num_tokens].lexeme, tokenList[num_tokens].type);
  num_tokens++;
  curr_token.type = 0;
  reset_lexeme();
  return 0;
}


// Main parsing loop (Jose Porta)
void parser(long f_sz, char input_arr[]) {
  int comment_state = 0;
  int parser_pos = 0;
  int currType = 0;
  int state = 0;

  for (int i = 0; i < f_sz; i++) {
    char currChar = input_arr[i];
    // Make sure nextChar can't go out of bounds (Trever Jones)
    char nextChar = (i + 1 < f_sz) ? input_arr[i + 1] : '\0';

    // Detect comments (Trever Jones)
    if (comment_state == 1) {
      if (currChar == '*' && nextChar == '/') {
        // Comment successfully closed (Trever Jones)
        comment_state = 0;
        // Skip '/' char (Trever Jones)
        i++;
        continue;
      }
      // Check that there are no more symbols left and comment is still open (Trever Jones)
      if (i == f_sz - 1) {
        printf("Error: Comment opened and not properly closed\n"); 
        comment_state = 0;
        // Reset position after opening '/*' (Trever Jones)
        i = parser_pos;
        curr_token.type = slashsym;
        curr_token.lexeme[strlen(curr_token.lexeme)] = '/';
        tokenize();
      }
      // Skip other chars while comment is open (Trever Jones)
      continue;
    }
    else {
      // Comment is opened (Trever Jones)
      if (currChar == '/' && nextChar == '*') {
        comment_state = 1;
        parser_pos = i;
        // Skip '*' char (Trever Jones)
        i++;
        continue;
      }
    }

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
      else if (isdigit(currChar)) {
        // Switch to number state
        state = numbersym;
        // Set current token to numbersym
        curr_token.type = state;
        // "rewind" loop to process char in as part of identifier
        i--;
        break;
      }

      // if currChar is a special character (Trever Jones)
      else if (is_sym(currChar) != 0) {
        // Switch to state based on specific symbol type
        state = is_sym(currChar);
        curr_token.type = state;
        // "rewind" loop to process char in as symbol
        i--;
        break;
      }

      // If currChar is a space, skip to next char (Trever Jones)
      else if (isspace(currChar)) {
        state = 0;
        break;
      }

      // Handle invalid symbol (Trever Jones)
      else {
        state = 0;
        printf("Error: Invalid symbol: '%s'\n", currChar);
        reset_lexeme();
      }
      break;

    case identsym:
      // identifiers have the form: letter(letter | digit)*
      // Add char to identifier name
      

      if (isalpha(currChar) || isdigit(currChar)) {
        curr_token.lexeme[strlen(curr_token.lexeme)] = currChar;
        // Check if identifier is a reserved word (returns identsym if not)
        state = is_reserved(curr_token.lexeme);
        if (state != identsym) {

          if (!isalpha(nextChar) && !isdigit(nextChar)) {
            i--;
            break;
          }

          // reserved word inside an identifier (Trever Jones)
          else {
            state = identsym;
            break;
          }
        }
        
      }

      // Non alpha-numeric value implies end of identifier
      if (!isalpha(nextChar) && !isdigit(nextChar)) {
        state = tokenize();

        break;
      }

      break;

    case numbersym:
      curr_token.lexeme[strlen(curr_token.lexeme)] = currChar;

      if (!isdigit(nextChar)) {
        tokenize();
        state = 0;
      }

      break;
    case lessym:
      // Handling neqsym (Trever Jones)
      if (nextChar == '>') {
        curr_token.type = neqsym;
        curr_token.lexeme[strlen(curr_token.lexeme)] = currChar;
        curr_token.lexeme[strlen(curr_token.lexeme)] = '>';
        tokenize();
        state = 0;
        i++;
        break;
      }
      // Handling leqsym (Trever Jones)
      else if (nextChar == '=') {
        curr_token.type = leqsym;
        curr_token.lexeme[strlen(curr_token.lexeme)] = currChar;
        curr_token.lexeme[strlen(curr_token.lexeme)] = '=';
        tokenize();
        state = 0;
        i++;
        break;
      }

      else {
        curr_token.lexeme[strlen(curr_token.lexeme)] = currChar;
        tokenize();
        state = 0;  
        break;
      }
    case gtrsym:
      // Handling geqsym 
      if (nextChar == '=') {
        curr_token.type = geqsym;
        curr_token.lexeme[strlen(curr_token.lexeme)] = currChar;
        curr_token.lexeme[strlen(curr_token.lexeme)] = '=';
        tokenize();
        state = 0;
        i++;
        break;
      }

      else {
        curr_token.lexeme[strlen(curr_token.lexeme)] = currChar;
        tokenize();
        state = 0;  
        break;
      }
    case becomessym:
    case plussym:
    case minussym:
    case multsym:
    case slashsym:
    case rparentsym:
    case lparentsym:
    case eqsym:
    case commasym:
    case periodsym:
    case semicolonsym:
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
        curr_token.type = state;
        state = tokenize();
        break;
      }
      // ':' symbol
    case 35:
      if (nextChar == '=')
      {
        curr_token.lexeme[strlen(curr_token.lexeme)] = currChar;
        curr_token.type = becomessym;
        state = becomessym;
        break;

      } else {
        printf("Error: Invalid symbol: '%s'\n", currChar);
        reset_lexeme();
        state = 0;
      }
      
    default:
      break;
    }
  }
}

int main(int argc, char *argv[]) {
  char *inputArr = NULL;

  FILE *file;
  file = fopen(argv[1], "rb");
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
    inputArr = (char *)malloc(fileSize);
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
  printf("Lexeme Token   Type\n");

  // Parse inputArr
  parser(fileSize, inputArr);
  free(inputArr);

  printf("\nToken List:\n");
  for (int i = 0; i < num_tokens; i++) {
    if (tokenList[i].type == identsym || tokenList[i].type == numbersym) {
      printf("%d %s ", tokenList[i].type, tokenList[i].lexeme);
    }
    else {
      printf("%d ", tokenList[i].type);
    }
  }

  return 0;
}