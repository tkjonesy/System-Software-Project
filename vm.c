/*
COP3402 Programming Project
Trever Jones
Jose Porta
*/

#include <stdio.h>

// Constants
// PAS length size (Jose Porta)
#define ARRAY_SIZE 500

// Instruction and Opr. enums (Jose Porta)
enum Instruct { LIT = 1, OPR, LOD, STO, CAL, INC, JMP, JPC, SYS };
enum Oprs { RTN = 0, ADD, SUB, MUL, DIV, EQL, NEQ, LSS, LEQ, GTR, GEQ };

int main(int argc, char *argv[]) {
  int PAS[ARRAY_SIZE] = {0};
  int OP, L, M;
  int index = 10;

  FILE *file;
  file = fopen(argv[1], "r");

  // Checking for correct number of argumnets passed (Trever Jones)
  if (argc != 2) {
    printf("Please use correct command: %s <inputFileName>\n", argv[0]);
    return 1;
  }

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
  int IR[3] = {0, 0, 0};

  // Fetch Cycle --
  while (PC != index) {
    IR[0] = PAS[PC];
    IR[1] = PAS[PC + 1];
    IR[2] = PAS[PC + 2];

    /* Execution happens here */

    PC += 3;
  }

  return 0;
}