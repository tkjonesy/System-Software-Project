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

// Parser macros (Jose Porta)
#define MAX_SYMBOL_TABLE_SIZE 500

// Instruction and Opr. enums (Jose Porta)
enum Instruct { LIT = 1, OPR, LOD, STO, CAL, INC, JMP, JPC, SYS };
enum Oprs { RTN = 0, ADD, SUB, MUL, DIV, EQL, NEQ, LSS, LEQ, GTR, GEQ, ODD };

// Token type enum (Jose Porta)
typedef enum {
  oddsym = 1,
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
} token_type;

// Reserved words (Jose Porta)
char *reserved[] = {"odd", "const", "var",   "begin", "end",  "if",
                    "fi",  "then",  "while", "do",    "read", "write"};
int res_enums[] = {oddsym, constsym, varsym,   beginsym, endsym,  ifsym,
                   fisym,  thensym,  whilesym, dosym,    readsym, writesym};

// Token struct (Jose Porta)
typedef struct Token {
  token_type type;
  char lexeme[100];
} Token;
Token curr_token = {0, ""};
// Token Table
Token tokenList[TOKEN_TBL_SZ];
int num_tokens = 0;
// Index for the parser (Trever Jones)
int parserPos = 0;

// Instruction set architecture (Trever Jones)
int cx = 0;
typedef struct Instructions {
  int OP;
  int L;
  int M;
} Instructions;

Instructions instructionList[500];

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
    if (!isalpha(c) && !isdigit(c) && !isspace(c)) {
      printf("Error: Invalid symbol: '%c'\n", c);
      exit(1);
    }
    return 0;
  }
}

// Checks if indentifier is a reserved word and if so, returns its type
int is_reserved(char identifier[]) {
  for (int i = 0; i < 12; i++) {
    if (strcmp(reserved[i], identifier) == 0) {
      return res_enums[i];
    }
  }
  return identsym;
}

// Helper function to set all 11 chars in lexeme to null char
void reset_lexeme() {
  for (int i = 0; i < MAX_ID_LEN; i++) {
    curr_token.lexeme[i] = '\0';
  }
}

// Push new token to token table and reset curr_token (Jose Porta)
int tokenize() {
  if (curr_token.type == identsym && strlen(curr_token.lexeme) > MAX_ID_LEN) {
    printf("Error: identifier '%s' execeeds max length (11)\n",
           curr_token.lexeme);
    exit(1);
    curr_token.type = 0;
    reset_lexeme();
    return 0;
  }

  else if (curr_token.type == numbersym &&
           strlen(curr_token.lexeme) > MAX_NUM_LEN) {
    printf("Error: number '%s' execeeds max length (5)\n", curr_token.lexeme);
    exit(1);
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
      // Check that there are no more symbols left and comment is still open
      // (Trever Jones)
      if (i == f_sz - 1) {
        printf("Error: Comment opened and not properly closed\n");
        exit(1);
        comment_state = 0;
        // Reset position after opening '/*' (Trever Jones)
        i = parser_pos;
        curr_token.type = slashsym;
        curr_token.lexeme[strlen(curr_token.lexeme)] = '/';
        tokenize();
      }
      // Skip other chars while comment is open (Trever Jones)
      continue;
    } else {
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
        printf("Error: Invalid symbol: '%c'\n", currChar);
        exit(1);
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
    case oddsym:
    case constsym:
    case varsym:
    // case procsym:
    // case callsym:
    case beginsym:
    case endsym:
    case ifsym:
    case fisym:
    case thensym:
    // case elsesym:
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
      if (nextChar == '=') {
        curr_token.lexeme[strlen(curr_token.lexeme)] = currChar;
        curr_token.type = becomessym;
        state = becomessym;
        break;

      } else {
        printf("Error: Invalid symbol: '%c'\n", currChar);
        exit(1);
        reset_lexeme();
        state = 0;
      }

    default:
      printf("Error: Invalid symbol:\n");
      exit(1);
      break;
    }
  }
}
/* END OF SCANNER */

/*
START OF PARSER
*/

// Debug mode switch (prints extra details) (Jose Porta)
int debug = 0;
// Symbol struct (Jose Porta)
typedef struct Symbol {
  int kind;      // const = 1, var = 2, proc = 3
  char name[10]; // name up to 11 chars
  int val;       // number (ASCII value)
  int level;     // L level
  int addr;      // M address
  int mark;      // to indicate unavailable or deleted
} Symbol;

// SYMBOL TABLE(Jose Porta)
Symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
int num_symbols = 0;

// SYM table search (Jose Porta)
int sym_tbl_srch(char string[]) {
  for (int i = 0; i < num_symbols; i++) {
    // If matching symbol exists return it's index (i)
    if (strcmp(string, symbol_table[i].name) == 0) {
      return i;
    }
  }
  // If no match is found return -1
  return -1;
}

// Function for fetching next token (Trever Jones)
void getNextToken() {
  curr_token = tokenList[parserPos++];
  if (debug == 1) {
    printf("Current Token: %s\n", curr_token.lexeme);
  }
}

// Parser error enumeration (Trever Jones)
typedef enum {
  missingPeriod = 1,
  declareMissingIden,
  symbolTaken,
  constMissingEqual,
  constMissingInt,
  declareMissingSemicolon,
  undefinedInden,
  ConstAltered,
  missingBecomesym,
  beginMissingEnd,
  ifMissingThen,
  ifMissingFi,
  whileMissingDo,
  conditionMissingOper,
  unclosedParenth,
  arithmeticError,
} errorCode;

// Error handling (Trever Jones)
void error(errorCode error) {
  switch (error) {
  case missingPeriod:
    printf("Error: program must end with period\n");
    exit(1);
  case declareMissingIden:
    printf("Error: const, var, and read keywords must be followed by an "
           "identifier: %s\n",
           curr_token.lexeme);
    exit(1);
  case symbolTaken:
    printf("Error: symbol name has already been declared %s\n",
           curr_token.lexeme);
    exit(1);
  case constMissingEqual:
    printf("Error: constants must be assigned with =\n");
    exit(1);
  case constMissingInt:
    printf("Error: constants must be assigned an integer value\n");
    exit(1);
  case declareMissingSemicolon:
    printf("Error: constant and variable declarations must be followed by a semicolon");
    exit(1);
  case undefinedInden:
    printf("Error: undeclared identifier: %s\n", curr_token.lexeme);
    exit(1);
  case ConstAltered:
    printf("Error: only variable values may be altered\n");
    exit(1);
  case missingBecomesym:
    printf("Error: assignment statements must use :=\n");
    exit(1);
  case beginMissingEnd:
    printf("Error: begin must be followed by end\n");
    exit(1);
  case ifMissingThen:
    printf("Error: if must be followed by then\n");
    exit(1);
  case ifMissingFi:
    printf("Error: fi must follow then.\n");
    exit(1);
  case whileMissingDo:
    printf("Error: while must be followed by do\n");
    exit(1);
  case conditionMissingOper:
    printf("Error: condition must contain comparison operator\n");
    exit(1);
  case unclosedParenth:
    printf("Error: right parenthesis must follow left parenthesis\n");
    exit(1);
  case arithmeticError:
    printf("Error: arithmetic equations must contain operands, parentheses, "
           "numbers, or symbols\n");
    exit(1);
  default:
    printf("Undefined error %d\n", error);
    exit(1);
  }
}

// Emit instruction function (Trever Jones)
void emit(int OP, int L, int M) {
  if (cx > 500) {
    printf("Error: code too long\n");
  }

  else {
    instructionList[cx].OP = OP;
    instructionList[cx].L = L;
    instructionList[cx].M = M;
    cx++;
  }
}

// Function for marking every symbol to 1 after execution (Trever Jones)
void markAllSymb() {
  for (int i = 0; i < num_symbols; i++) {
    symbol_table[i].mark = 1;
  }
}

// Function signatures (Jose Porta)
void FACTOR();
void TERM();
void EXPRESSION();
// Grammar functions (Trever Jones / Jose Porta)

// Factor parser and code generation (Trever Jones)
void FACTOR() {
  if (curr_token.type == identsym) {
    int symIdx = sym_tbl_srch(curr_token.lexeme);

    // Verify identity is defined
    if (symIdx == -1) {
      error(undefinedInden);
    }

    // Identifier is a const (Trever Jones)
    if (symbol_table[symIdx].kind == 1) {
      emit(LIT, 0, symbol_table[symIdx].val);
    }

    // Identifier is a var (Trever Jones)
    else {
      emit(LOD, 0, symbol_table[symIdx].addr);
    }
    getNextToken();
  } else if (curr_token.type == numbersym) {
    // Emit LIT with lexeme as int
    emit(LIT, 0, atoi(curr_token.lexeme));
    getNextToken();
  } else if (curr_token.type == lparentsym) {
    getNextToken();
    EXPRESSION();
    if (curr_token.type != rparentsym) {
      error(unclosedParenth);
    }
    getNextToken();
  } else {
    error(arithmeticError);
  }
}

// Term parser and code generation (Trever Jones)
void TERM() {
  Token operand;
  // Required factor in term
  FACTOR();

  // Optional factors in term
  while (curr_token.type == multsym || curr_token.type == slashsym) {
    operand = curr_token;
    getNextToken();
    FACTOR();

    if (operand.type == multsym) {
      emit(OPR, 0, MUL);
    }

    else {
      emit(OPR, 0, DIV);
    }
  }
}

// Expression parser and code generation (Trever Jones)
void EXPRESSION() {
  Token operand;
  // Required term in expression
  TERM();

  // Optional terms
  while (curr_token.type == plussym || curr_token.type == minussym) {
    operand = curr_token;
    getNextToken();
    TERM();

    if (operand.type == plussym) {
      emit(OPR, 0, ADD);
    }

    else {
      emit(OPR, 0, SUB);
    }
  }
}

// CONDITIONS parsing & code gen (Jose Porta)
void CONDITION() {
  // ODD
  if (curr_token.type == oddsym) {
    getNextToken();
    // Parse expression after "odd"
    EXPRESSION();

    // Verify there's no extra/incorrect tokens in "odd" conditional (Jose Porta)
    if (curr_token.type != dosym && curr_token.type != thensym) {
      error(arithmeticError);
    }

    emit(OPR, 0, ODD);
  } else {

    EXPRESSION();
    switch (curr_token.type) {
    // EQUAL (Jose Porta)
    case eqsym:
      getNextToken();
      EXPRESSION();
      emit(OPR, 0, EQL);
      break;
    // NOT EQUAL (Jose Porta)
    case neqsym:
      getNextToken();
      EXPRESSION();
      emit(OPR, 0, NEQ);
      break;
    // LESS THAN (Jose Porta)
    case lessym:
      getNextToken();
      EXPRESSION();
      emit(OPR, 0, LSS);
      break;
    // LESS THAN OR EQ TO (Jose Porta)
    case leqsym:
      getNextToken();
      EXPRESSION();
      emit(OPR, 0, LEQ);
      break;
    // GREATER THAN (Jose Porta)
    case gtrsym:
      getNextToken();
      EXPRESSION();
      emit(OPR, 0, GTR);
      break;
    // GREATER THAN OR EQ TO (Jose Porta)
    case geqsym:
      getNextToken();
      EXPRESSION();
      emit(OPR, 0, GEQ);
      break;
    // Invalid Comparison (Jose Porta)
    default:
      error(conditionMissingOper);
      break;
    }
  }
}

// STATEMENT parsing and codegen (Jose Porta)
void STATEMENT() {
  int sym_idx;
  int jpc_idx;
  switch (curr_token.type) {
  // VARIABLE MODIFICATION
  case identsym:
    sym_idx = sym_tbl_srch(curr_token.lexeme);

    // Undeclared variable
    if (sym_idx == -1) {
      error(undefinedInden);
    }
    // Identifier is not a variable
    if (symbol_table[sym_idx].kind != 2) {
      error(ConstAltered);
    }

    getNextToken();

    // variable identifier is not follow by becomes
    if (curr_token.type != becomessym) {
      error(missingBecomesym);
    }

    getNextToken();
    EXPRESSION();
    emit(STO, 0, symbol_table[sym_idx].addr);
    break;

  // BEGIN statement declaration (Jose Porta)
  case beginsym:
    getNextToken();
    // Mandatory statement after "begin"
    STATEMENT();

    // Optional statements in "begin"
    while (curr_token.type == semicolonsym) {
      getNextToken();
      STATEMENT();
    }

    // "begin" must be followed by "end"
    if (curr_token.type != endsym) {
      error(beginMissingEnd);
    }

    getNextToken();
    break;

  // IF STATEMENT (Jose Porta)
  case ifsym:
    getNextToken();
    // Get condition after "if"
    CONDITION();
    // Position of conditional jump in code array
    jpc_idx = cx;

    // JPC with dummy location
    emit(JPC, 0, 0);

    // Missing "then" after "if"
    if (curr_token.type != thensym) {
      error(ifMissingThen);
    }

    getNextToken();
    STATEMENT();

    // Missing "fi" after "if"
    if (curr_token.type != fisym) {
      error(ifMissingFi);
    }

    // Update JPC with location after conditional statement
    // (location is mult. by 3 to account for PC in VM)
    instructionList[jpc_idx].M = cx * 3;
    getNextToken();
    break;

  // WHILE LOOP (Jose Porta)
  case whilesym:
    getNextToken();
    // Location of loop start in code array
    // (location is mult. by 3 to account for PC in VM)
    int loop_idx = cx * 3;
    CONDITION();

    // Check for "do" following "while"
    if (curr_token.type != dosym) {
      error(whileMissingDo);
    }

    getNextToken();
    // Position of conditional jump in code array
    jpc_idx = cx;

    // JPC with dummy location
    emit(JPC, 0, 0);
    STATEMENT();

    // JMP to loop start
    emit(JMP, 0, loop_idx);

    // Update JPC with location after while statement
    // (location is mult. by 3 to account for PC in VM)
    instructionList[jpc_idx].M = cx * 3;
    break;

  // READ INPUT (Jose Porta)
  case readsym:
    getNextToken();
    // Missing variable after READ
    if (curr_token.type != identsym) {
      error(declareMissingIden);
    }

    sym_idx = sym_tbl_srch(curr_token.lexeme);

    // Undefined identifier
    if (sym_idx == -1) {
      error(undefinedInden);
    }

    // Identifier is not a variable
    if (symbol_table[sym_idx].kind != 2) {
      error(ConstAltered);
    }

    getNextToken();

    // Emit READ instruction
    emit(SYS, 0, 2);
    // Store input value in variable provided
    emit(STO, 0, symbol_table[sym_idx].addr);
    break;

  // WRITE OUTPUT (Jose Porta)
  case writesym:
    getNextToken();
    EXPRESSION();

    // Emit WRITE instruction
    emit(SYS, 0, 1);
    break;

  // Empty statement case
  default:
    break;
  }
}

// VARIABLE DECLARATION (Trever Jones)
int VAR_DECL() {
  int num_vars = 0;
  // Check if var (optional) is declared
  if (curr_token.type == varsym) {
    // Variable must be followed by at least 1 identifier if declared
    do {
      // Update var count
      num_vars++;
      getNextToken();
      if (curr_token.type != identsym) {
        error(declareMissingIden);
      }
      // Check if identifier is taken
      int ident_idx = sym_tbl_srch(curr_token.lexeme);
      if (ident_idx != -1) {
        error(symbolTaken);
      }

      // Add variable to symbol table (Jose Porta)
      symbol_table[num_symbols].kind = 2;
      strcpy(symbol_table[num_symbols].name, curr_token.lexeme);
      symbol_table[num_symbols].val = 0;
      symbol_table[num_symbols].level = 0;
      symbol_table[num_symbols].addr = num_vars + 2;
      symbol_table[num_symbols].mark = 0;
      num_symbols++;
      getNextToken();

    } while (curr_token.type == commasym);

    // Declaration statement must end with ';'
    if (curr_token.type != semicolonsym) {
      error(declareMissingSemicolon);
    }
    getNextToken();
  }

  return num_vars;
}

// Constant declaration parser and code generation (Trever Jones)
void CONST_DECL() {
  if (curr_token.type == constsym) {

    do {
      getNextToken();

      // Const must be followed by identifier
      if (curr_token.type != identsym) {
        error(declareMissingIden);
      }

      // Check if symbol has already been declared
      if (sym_tbl_srch(curr_token.lexeme) != -1) {
        error(symbolTaken);
      }

      // Save const identifier (Trever Jones)
      char iden[10];
      strcpy(iden, curr_token.lexeme);

      getNextToken();

      // Verify const is declared with '='
      if (curr_token.type != eqsym) {
        error(constMissingEqual);
      }

      getNextToken();
      // Verify const is set to numerical value
      if (curr_token.type != numbersym) {
        error(constMissingInt);
      }

      // Add const to symbol table (Trever Jones)
      symbol_table[num_symbols].kind = 1;
      strcpy(symbol_table[num_symbols].name, iden);
      symbol_table[num_symbols].val = atoi(curr_token.lexeme);
      symbol_table[num_symbols].mark = 0;
      num_symbols++;

      getNextToken();
    } while (curr_token.type == commasym);

    if (curr_token.type != semicolonsym) {
      error(declareMissingSemicolon);
    }
    getNextToken();
  }
}

// Block parser and code generation (Trever Jones)
void BLOCK() {
  CONST_DECL();
  int numVars = VAR_DECL();
  emit(INC, 0, (3 + numVars));
  STATEMENT();
}

// Program parser and code generation (Trever Jones)
void PROGRAM() {
  emit(JMP, 0, 3);
  getNextToken();
  BLOCK();
  if (curr_token.type != periodsym) {
    error(1);
  }
  emit(SYS, 0, 3);
  // Mark all with 1, execution is over (Trever Jones)
  markAllSymb();
}

// Print semantic repr. of OP code (Jose Porta)
void print_op(int op) {
  // LIT = 1, OPR, LOD, STO, CAL, INC, JMP, JPC, SYS };
  switch (op) {
  case LIT:
    printf("LIT");
    break;
  case OPR:
    printf("OPR");
    break;
  case LOD:
    printf("LOD");
    break;
  case STO:
    printf("STO");
    break;
  case CAL:
    printf("CAL");
    break;
  case INC:
    printf("INC");
    break;
  case JMP:
    printf("JMP");
    break;
  case JPC:
    printf("JPC");
    break;
  case SYS:
    printf("SYS");
    break;
  default:
    break;
  }
}

int main(int argc, char *argv[]) {
  char *inputArr = NULL;

  FILE *file;
  FILE *output;
  const char *inputFilename = argv[1];
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
  printf("Lexeme Token   Type\n");

  // Parse inputArr
  parser(fileSize, inputArr);
  free(inputArr);

  printf("\nToken List:\n");
  for (int i = 0; i < num_tokens; i++) {
    if (tokenList[i].type == identsym || tokenList[i].type == numbersym) {
      printf("%d %s ", tokenList[i].type, tokenList[i].lexeme);
    } else {
      printf("%d ", tokenList[i].type);
    }
  }
  printf("\n");

  // Run code generation (Trever Jones)
  PROGRAM();

  // Output OP code to file (Trever Jones)
  char outputFilename[50];
  snprintf(outputFilename, sizeof(outputFilename), "Assembly-Output-%s",
           inputFilename);
  output = fopen(outputFilename, "w");

  // Print OP codes (Jose Porta)
  printf("\nAssembly Code:\n\n");
  printf("Line\tOP\tL\tM\n");
  for (int i = 0; i < cx; i++) {
    // Push OP code to output file (Trever Jones)
    if (i != 0) {
      fprintf(output, "\n%d %d %d", instructionList[i].OP, instructionList[i].L,
              instructionList[i].M);
    } else {
      fprintf(output, "%d %d %d", instructionList[i].OP, instructionList[i].L,
              instructionList[i].M);
    }

    // Print code to screen (Jose Porta)
    Instructions inst = instructionList[i];
    printf("%d\t", i);
    print_op(inst.OP);
    printf("\t%d\t%d\n", inst.L, inst.M);
  }
  fclose(output);

  // Print Symbol table (Trever Jones)
  printf("\nSymbol Table:\n\n");
  printf("%-5s | %-11s | %-5s | %-5s | %-7s | %-5s\n", "Kind", "Name", "Value",
         "Level", "Address", "Mark");
  printf("-------------------------------------------------------------\n");
  for (int i = 0; i < num_symbols; i++) {
    printf("%-5d | %-11s | %-5d | %-5d | %-7d | %-5d\n", symbol_table[i].kind,
           symbol_table[i].name, symbol_table[i].val, symbol_table[i].level,
           symbol_table[i].addr, symbol_table[i].mark);
  }

  return 0;
}