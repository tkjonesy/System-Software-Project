/*
COP3402 Programming Project HW2
Trever Jones
Jose Porta
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
  elsesym,
  colonsym
} token_type;

// Reserved words (Jose Porta)
char *reserved[] = {"const", "var", "procedure", "call", "begin",
                    "end",   "if",  "fi",        "then", "else",
                    "while", "do",  "read",      "write"};
int res_enums[] = {constsym, varsym, procsym, callsym, beginsym,
                    endsym,   ifsym,  fisym, thensym, elsesym,
                    whilesym, dosym,  readsym, writesym};

// Token struct (Jose Porta)
typedef struct Token {
  token_type type;
  char lexeme[MAX_ID_LEN + 1];
} Token;
Token curr_token = {0, ""};
// Token Table
Token *tokenList = NULL;
int num_tokens = 0;

// Determine Special Symbol type
int is_sym(char c) {
      switch (c){
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
int is_reserved(char identifier[]){
  for(int i=0; i < 14; i++){
    if(strcmp(reserved[i], identifier) == 0){
      return res_enums[i];
    }
  }
  return identsym;
}

// Push new token to token table
void tokenize(){
  tokenList[num_tokens] = curr_token;
  printf("Token added: %s\n", tokenList[num_tokens]);
  num_tokens++;
  curr_token.type = 0;
  for (int i = 0; i < MAX_ID_LEN; i++){
        curr_token.lexeme[i] = '\0';
    }
}

// Main parsing loop (Jose Porta)
void parser(long f_sz, char input_arr[]) {
  int comment_state = 0;
  int parser_pos = 0;
  int currType = 0;
  int state = 0;

  // i < f_sz prints extra chars and f_sz - 10 fixes it for some reason? (delete before submission)
  for (int i = 0; i < f_sz-10 ; i++) {
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

    switch(state){
      case 0:
        // If currChar is a letter -> state = identifier
        if (isalpha(currChar)) {
          state = identsym;
          curr_token.type = identsym;
          // Add char to identifier name
          curr_token.lexeme[strlen(curr_token.lexeme)] = currChar;
          break;
        }
        
        // If currChar is a digit -> state = number
        else if (isdigit(currChar)) {
          state = numbersym;
          curr_token.type = numbersym;
          curr_token.lexeme[strlen(curr_token.lexeme)] = currChar;
        }

        // if currChar is a special character (Trever Jones)
        else if (is_sym(currChar)) {
          state = is_sym(currChar);
          // Add to token table, reset curr_token (Trever Jones)
        }

        // If currChar is a space (Trever Jones)
        else if (isspace(currChar)) {
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
        
        if(isalpha(currChar) || isdigit(currChar)) {
          // Add char to identifier name
          curr_token.lexeme[strlen(curr_token.lexeme)] = currChar;

          // Need to implement function to check if name is reserved
          // if (reserved(curr_token.lexeme) -> state = constsym or ifsym or varsym, etc.)
          if(is_reserved(curr_token.lexeme)){
            state = is_reserved(curr_token.lexeme);
            break;
          }

          if (!isalpha(nextChar) || !isdigit(nextChar)){
            // Add token to token table and reset curr_token
            // This should be a separate function
            tokenize();
            state = 0;
          } else if (strlen(curr_token.lexeme) > MAX_ID_LEN){
            printf("Error: identifier %s execeeds max length (11)", curr_token.lexeme);
            
          }
        } else {
          tokenize();
          state = 0;
        }
        break;

      case numbersym:
        curr_token.lexeme[strlen(curr_token.lexeme)] = currChar;
        if (!isdigit(currChar)){
          tokenize();
          state = 0;
        } 
        
        if (strlen(curr_token.lexeme) > MAX_NUM_LEN){
            printf("Error: number %s execeeds max length (5)", curr_token.lexeme);
            
          }
      break;
      default:
      break;
    }
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
  
  // File name passed incorrectly as argumnent or file does not exsist (Trever Jones)
  else {
    printf("No file named %s found\n", argv[1]);
    return 1;
  }
  
  tokenList[fileSize];

  printf("Source Program:\n%s\n\n", inputArr);
  printf("Lexeme Table:\n\n");

  // Parse inputArr
  parser(fileSize, inputArr);
  free(inputArr);
  return 0;
}