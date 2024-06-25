# Homework 2: Token Scanner

## Description

This project is lexical parser for PL/0. The program takes PL/0 program (.txt) as input and parses for tokens based
on the following lexical grammar:

ident ::= letter {letter | digit}.
letter ::= "a" | "b" | ... | "y" | "z" | "A" | "B" | ... | "Y" | "Z".
number ::= digit {digit}.
digit ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9“.

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
`gcc -o lex lex.c`

Ensure input file is in the same directory then run the .exe passing the input file name:<br>
`./lex.exe <inputFileName>`

## Example

```bash
# Example command and expected output
./lex input.txt
Output: Result of processing input.txt
Source Program:
.xvar w, x, 5;
read w;
beginy
555555
   x:= 4;
   /* testing */
   if w > x then
        w:= w + 1
   else
   /*test */
        w := x;
   fi
   /*
   ><<><>>
   =
end
write w.

Lexeme Table:

Lexeme Token   Type
.              19
xvar           2
w              2
,              17
x              2
,              17
5              3
;              18
read           32
w              2
;              18
beginy         2
Error: number '555555' execeeds max length (5)
x              2
:=             20
4              3
;              18
if             23
w              2
>              13
x              2
then           24
w              2
:=             20
w              2
+              4
1              3
else           33
w              2
:=             20
x              2
;              18
fi             8
Error: Comment opened and not properly closed
/              7
*              6
>              13
<              11
<>             10
<>             10
>              13
=              9
end            22
write          31
w              2
.              19

Token List:
19 2 xvar 2 w 17 2 x 17 3 5 18 32 2 w 18 2 beginy 2 x 20 3 4 18 23 2 w 13 2 x 24 2 w 20 2 w 4 3 1 33 2 w 20 2 x 18 8 7 6 13 11 10 10 13 9 22 31 2 w 19 
```

## Team Members

Team Member 1: Trever Jones, tr755969@ucf.edu<br>
Team Member 2: Jose Porta, Jose.porta@ucf.edu<br>

## Contact Information

For any questions or issues, please contact:

Name: Trever Jones<br>
Email: tr755969@ucf.edu
