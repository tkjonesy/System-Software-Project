/*
COP3402 Programming Project
Trever Jones
Jose Porta
*/

#include <stdio.h>

// Constants
// PAS length size (Jose Porta)
#define ARRAY_SIZE 500
int PAS[ARRAY_SIZE] = {0};

// Instruction and Opr. enums (Jose Porta)
enum Instruct { LIT = 1, OPR, LOD, STO, CAL, INC, JMP, JPC, SYS };
enum Oprs { RTN = 0, ADD, SUB, MUL, DIV, EQL, NEQ, LSS, LEQ, GTR, GEQ };

// Instruct names as strings (Jose Porta)
// "N/A" is a place holder so string indexes align with Instruct enum
char i_names[10][3] = {"N/A", "LIT", "OPR", "LOD", "STO",
                       "CAL", "INC", "JMP", "JPC", "SYS"};
char opr_names[11][3] = {"RTN", "ADD", "SUB", "MUL", "DIV", "EQL",
                         "NEQ", "LSS", "LEQ", "GTR", "GEQ"};

// Struct for Instruction Register (Trever Jones)
typedef struct InstructionRegister {
  int OP;
  int L;
  int M;
} InstructionRegister;

// Find base L levels down, PROVIDED FUNCTION (Trever Jones)
int base(int BP, int L) {
  // arb = activation record base
  int arb = BP;

  // find base L levels down
  while (L > 0) {
    arb = PAS[arb];
    L--;
  }
  return arb;
}

int main(int argc, char *argv[]) {
  int OP, L, M;
  int index = 10;

  FILE *file;
  file = fopen(argv[1], "r");

  // Checking for correct number of argumnets passed (Trever Jones)
  if (argc != 2) {
    printf("Please use correct command: %s <inputFileName>\n", argv[0]);
    return 1;
  }

  // Reading in input from input file (Trever Jones)
  if (file != NULL) {
    while (fscanf(file, "%d %d %d", &OP, &L, &M) == 3) {
      PAS[index] = OP;
      PAS[index + 1] = L;
      PAS[index + 2] = M;
      index += 3;
    }

  }
  // File name passed incorrectly as argumnent or file does not exsist (Trever
  // Jones)
  else {
    printf("No file named %s found\n", argv[1]);
  }

  // Register initialization (Jose Porta)
  int BP = 499;
  int SP = 500;
  int PC = 10;
  InstructionRegister IR;
  IR.OP = 0;
  IR.L = 0;
  IR.M = 0;

  // Initial VM status message (Jose Porta)
  printf("%20s %10s %10s %10s", "PC", "BP", "SP", "stack\n");
  printf("Initial values\t %-10d %-10d %-10d\n\n", PC, BP, SP);

  // FETCH CYCLE -- index = end of "text" section of PAS (Jose Porta)
  while (PC != index) {
    IR.OP = PAS[PC];
    IR.L = PAS[PC + 1];
    IR.M = PAS[PC + 2];
    PC += 3;

    // EXECUTION CYCLE (Trever Jones / Jose Porta)
    switch (IR.OP) {
    case LIT:
      SP--;
      PAS[SP] = IR.M;

      break;
    case OPR:
      /* code */
      break;
    case LOD:
      SP--;
      PAS[SP] = PAS[base(BP, IR.L) - IR.M];

      break;
    case STO:
      PAS[base(BP, IR.L) - IR.M] = PAS[SP];

      SP++;
      break;
    case CAL:
      /* code */
      break;
    case INC:
      SP = SP - IR.M;

      break;
    case JMP:
      /* code */
      break;
    case JPC:
      /* code */
      break;
    case SYS:
      /* code */
      break;
    default:
      printf("Invalid Instruction.\n");
      break;
    }
  }

  return 0;
}