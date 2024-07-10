# Homework 3: Tiny PL/0 Compiler

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
term ::= factor {("*"|"/") factor}.<br>
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
./hw4compiler validInput.txt

# Output: Result of processing validInput.txt
Source Program:
const a = 2, b = 2;
var x, y, z;
begin
read x;
y := x + a;
z := y * b;
while z > 0 do
  z := z - 1;
if z = 0 then
  write z
  fi;
write (z + a) * (b / a)
end.

Lexeme Table:

Lexeme Token   Type
const          28
a              2
=              9
2              3
,              17
b              2
=              9
2              3
;              18
var            29
x              2
,              17
y              2
,              17
z              2
;              18
begin          21
read           32
x              2
;              18
y              2
:=             20
x              2
+              4
a              2
;              18
z              2
:=             20
y              2
*              6
b              2
;              18
while          25
z              2
>              13
0              3
do             26
z              2
:=             20
z              2
-              5
1              3
;              18
if             23
z              2
=              9
0              3
then           24
write          31
z              2
fi             8
;              18
write          31
(              15
z              2
+              4
a              2
)              16
*              6
(              15
b              2
/              7
a              2
)              16
end            22
.              19

Token List:
28 2 a 9 3 2 17 2 b 9 3 2 18 29 2 x 17 2 y 17 2 z 18 21 32 2 x 18 2 y 20 2 x 4 2 a 18 2 z 20 2 y 6 2 b 18 25 2 z 13 3 0 26 2 z 20 2 z 5 3 1 18 23 2 z 9 3 0 24 31 2 z 8 18 31 15 2 z 4 2 a 16 6 15 2 b 7 2 a 16 22 19

Assembly Code:

Line    OP      L       M
0       JMP     0       3
1       INC     0       6
2       SYS     0       2
3       STO     0       3
4       LOD     0       3
5       LIT     0       2
6       OPR     0       1
7       STO     0       4
8       LOD     0       4
9       LIT     0       2
10      OPR     0       3
11      STO     0       5
12      LOD     0       5
13      LIT     0       0
14      OPR     0       9
15      JPC     0       63
16      LOD     0       5
17      LIT     0       1
18      OPR     0       2
19      STO     0       5
20      JMP     0       36
21      LOD     0       5
22      LIT     0       0
23      OPR     0       5
24      JPC     0       81
25      LOD     0       5
26      SYS     0       1
27      LOD     0       5
28      LIT     0       2
29      OPR     0       1
30      LIT     0       2
31      LIT     0       2
32      OPR     0       4
33      OPR     0       3
34      SYS     0       1
35      SYS     0       3

Symbol Table:

Kind  | Name        | Value | Level | Address | Mark
-------------------------------------------------------------
1     | a           | 2     | 0     | 0       | 1
1     | b           | 2     | 0     | 0       | 1
2     | x           | 0     | 0     | 3       | 1
2     | y           | 0     | 0     | 4       | 1
2     | z           | 0     | 0     | 5       | 1
```

## Team Members

Team Member 1: Trever Jones, tr755969@ucf.edu<br>
Team Member 2: Jose Porta, Jose.porta@ucf.edu<br>

## Contact Information

For any questions or issues, please contact:

Name: Trever Jones<br>
Email: tr755969@ucf.edu
