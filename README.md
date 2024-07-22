# Homework 4: Tiny PL/0 Compiler

## Description

This project takes the tokens produced by the scanner and passed to the parser to check syntax, populate a symbol table, and output an ISA to pass to the virtual machine created in HW1.

EBNF of PL/0:<br>

program ::= block "." .<br>
block ::= const-declaration var-declaration procedure-declaration statement.<br>
constdeclaration ::= ["const" ident "=" number {"," ident "=" number} ";"].<br>
var-declaration ::= [ "var "ident {"," ident} “;"].<br>
procedure-declaration ::= { "procedure" ident ";" block ";" }<br>
statement ::= [ ident ":=" expression<br>
| "call" ident<br>
| "begin" statement { ";" statement } "end"<br>
| "if" condition "then" statement "fi"<br>
| "while" condition "do" statement<br>
| "read" ident<br>
| "write" expression<br>
| empty ] .<br>
condition ::= "odd" expression<br>
| expression rel-op expression.<br>
rel-op ::= "="|“< >"|"<"|"<="|">"|">=“.<br>
expression ::= term { ("+"|"-") term}.<br>
term ::= factor {("\*"|"/") factor}.<br>
factor ::= ident | number | "(" expression ")“.<br>
number ::= digit {digit}.<br>
ident ::= letter {letter | digit}.<br>
digit ;;= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9“.<br>
letter ::= "a" | "b" | … | "y" | "z" | "A" | "B" | ... | "Y" | "Z".<br>
Based on Wirth’s definition for EBNF we have the following rule:<br>
[ ] means an optional item.<br>
{ } means repeat 0 or more times.<br>
Terminal symbols are enclosed in quote marks.<br>
A period is used to indicate the end of the definition of a syntactic class.<br>

## Table of Contents

- [Getting Started](#getting-started)
- [Running The Code](#running-the-code)
- [Example](#example)
- [Team Members](#team-members)
- [Contact Information](#contact-information)

## Getting Started

Clone the repository:<br>
`git clone https://github.com/tkjonesy/System-Software-Project.git`

## Running the Code

Open your terminal of choice.<br>

Navigate to the folder: <br>
`cd ...\System Software Project\`

Compile the code:<br>
`gcc -o hw4compiler hw4compiler.c`

Ensure input file is in the same directory then run the .exe passing the input file name:<br>
`./hw4compiler <inputFileName>`

## Example

```
# Example command and expected output
./hw4compiler input.txt

# Output: Result of processing validInput.txt
Source Program:
var f, n;
procedure fact;
    var ans1;
    begin
        ans1:=n;
        n:= n-1;
        if n = 0 then f := 1fi;
        if n > 0 then call fact fi;
        f:=f*ans1;
    end;
begin
    n:=4;
    call fact;
    write f
end.


No errors, program is syntactically correct

Assembly Code:

Line    OP      L       M
0       JMP     0       85
1       JMP     0       16
2       INC     0       4
3       LOD     1       4
4       STO     0       3
5       LOD     1       4
6       LIT     0       1
7       OPR     0       2
8       STO     1       4
9       LOD     1       4
10      LIT     0       0
11      OPR     0       5
12      JPC     0       55
13      LIT     0       1
14      STO     1       3
15      LOD     1       4
16      LIT     0       0
17      OPR     0       9
18      JPC     0       70
19      CAL     1       13
20      LOD     1       3
21      LOD     0       3
22      OPR     0       3
23      STO     1       3
24      OPR     0       0
25      INC     0       5
26      LIT     0       4
27      STO     0       4
28      CAL     0       13
29      LOD     0       3
30      SYS     0       1
31      SYS     0       3
```

## Team Members

Team Member 1: Trever Jones, tr755969@ucf.edu<br>
Team Member 2: Jose Porta, Jose.porta@ucf.edu<br>

## Contact Information

For any questions or issues, please contact:

Name: Trever Jones<br>
Email: tr755969@ucf.edu
