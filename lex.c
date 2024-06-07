/*
COP3402 Programming Project HW2
Trever Jones
Jose Porta
*/

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

int main() {}