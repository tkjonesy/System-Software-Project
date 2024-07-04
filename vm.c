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
char i_names[10][4] = {"N/A", "LIT", "OPR", "LOD", "STO",
                       "CAL", "INC", "JMP", "JPC", "SYS"};
char opr_names[11][4] = {"RTN", "ADD", "SUB", "MUL", "DIV", "EQL",
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
  int index = 0;

  FILE *file;
  file = fopen(argv[1], "r");

  // Checking for correct number of argumnets passed (Trever Jones)
  if (argc != 2) {
    printf("Please use correct command: %s <inputFileName>\n", argv[0]);
    return 1;
  }

  // Reading in input from input file (Trever Jones)
  if (file != NULL) {
    while (fscanf(file, " %d %d %d", &OP, &L, &M) == 3) {
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
    return 1;
  }

  // Register initialization (Jose Porta)
  int BP = 499;
  int SP = 500;
  int PC = 0;

  // An array to keep track AR borders for printing "|" (Trever Jones)
  int BPHistory[100];
  int BPHistorySize = 0;

  // End of Program flag (Jose Porta)
  int eop = 0;

  // Initialize instruction register (Trever Jones)
  InstructionRegister IR;
  IR.OP = 0;
  IR.L = 0;
  IR.M = 0;

  // Initial VM status message (Jose Porta)
  printf("%-16s%-8s%-8s%-8s%s\n", "", "PC", "BP", "SP", "Stack");
  printf("Initial values: %-8d%-8d%d\n\n", PC, BP, SP);

  // FETCH CYCLE -- index = end of "text" section of PAS (Jose Porta)
  while (eop != 1) {
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

    // Arithmetic Operations (Jose Porta)
    case OPR:
      switch (IR.M) {
      case RTN:
        SP = BP + 1;
        BP = PAS[SP - 2];
        PC = PAS[SP - 3];

        break;

      case ADD:
        PAS[SP + 1] = PAS[SP + 1] + PAS[SP];
        SP++;
        break;

      case SUB:
        PAS[SP + 1] = PAS[SP + 1] - PAS[SP];
        SP++;
        break;

      case MUL:
        PAS[SP + 1] = PAS[SP + 1] * PAS[SP];
        SP++;
        break;

      case DIV:
        PAS[SP + 1] = PAS[SP + 1] / PAS[SP];
        SP++;
        break;

      case EQL:
        PAS[SP + 1] = PAS[SP + 1] == PAS[SP];
        SP++;
        break;

      case NEQ:
        PAS[SP + 1] = PAS[SP + 1] != PAS[SP];
        SP++;
        break;

      case LSS:
        PAS[SP + 1] = PAS[SP + 1] < PAS[SP];
        SP++;
        break;

      case LEQ:
        PAS[SP + 1] = PAS[SP + 1] <= PAS[SP];
        SP++;
        break;

      case GTR:
        PAS[SP + 1] = PAS[SP + 1] > PAS[SP];
        SP++;
        break;

      case GEQ:
        PAS[SP + 1] = PAS[SP + 1] > PAS[SP];
        SP++;
        break;

      default:
        break;
      }

      break;
      // End Math Ops

    case LOD:
      SP--;
      PAS[SP] = PAS[base(BP, IR.L) - IR.M];
      break;

    case STO:
      PAS[base(BP, IR.L) - IR.M] = PAS[SP];
      SP++;
      break;

    case CAL:
      PAS[SP - 1] = base(BP, IR.L);
      PAS[SP - 2] = BP;
      PAS[SP - 3] = PC;

      BP = SP - 1;
      PC = IR.M;
      break;

    case INC:
      SP = SP - IR.M;

      break;

    case JMP:
      PC = IR.M;
      break;

    case JPC:
      if (PAS[SP] == 0) {
        PC = IR.M;
      }
      SP++;

      break;

    // SYSTEM CALLS (Jose Porta)
    case SYS:
      switch (IR.M) {
      case 1: // OUTPUT
        printf("Output result is: %d\n", PAS[SP]);
        SP++;
        break;

      case 2: // INPUT
        SP--;
        printf("Please enter an integer: ");
        fscanf(stdin, " %d", &PAS[SP]);
        break;

      case 3: // HALT
        eop = 1;
        break;

      default:
        break;
      }
      break;
    // End SYS Calls
    default:
      printf("Invalid Instruction.\n");
      break;
    }
    if (IR.OP == OPR && IR.M == RTN) {
      // Remove border from AR array
      BPHistorySize--;
    }
    if (IR.OP == CAL) {
      // Add BP to AR border array (Trever Jones)
      BPHistory[BPHistorySize++] = BP;
    }

    // VM status output (Jose Porta / Trever Jones)
    if (IR.OP != 2) {
      printf("  %s %d %-8d", i_names[IR.OP], IR.L, IR.M);
    } else {
      printf("  %s %d %-8d", opr_names[IR.M], IR.L, IR.M);
    }

    printf("%-8d%-8d%-8d", PC, BP, SP);

    for (int i = 499; i >= SP; i--) {
      printf("%d ", PAS[i]);

      // Checking for boundry between activation records (Trever Jones)
      for (int j = 0; j < BPHistorySize; j++) {
        if (i == BPHistory[j] + 1 && i != SP) {
          printf("| ");
        }
      }
    }
    printf("\n");
  }

  return 0;
}