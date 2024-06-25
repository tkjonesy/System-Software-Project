# Homework 3: Tiny PL/0 Compiler

## Description

This project is capable of getting the tokens produced by the scanner and passed to the parser to check syntax, populate a symbol table, and output an ISA to pass to the virtual machine created in HW1.

EBNF of  tiny PL/0:

program ::= block "." . 
block ::= const-declaration  var-declaration  statement.	
constdeclaration ::= [ “const” ident "=" number {"," ident "=" number} “;"].	
var-declaration  ::= [ "var" ident {"," ident} “;"].
statement   ::= [ ident ":=" expression
	      	| "begin" statement { ";" statement } "end" 
	      	| "if" condition "then" statement "fi"
		| "while" condition "do" statement
| "read" ident 
		| "write"  expression 
	      	| empty ] .  
condition ::= "odd" expression 
	  	| expression  rel-op  expression.  
rel-op ::= "="|“<>"|"<"|"<="|">"|">=“.
expression ::=  term { ("+"|"-") term}.
term ::= factor {("*"|"/") factor}. 
factor ::= ident | number | "(" expression ")“.
number ::= digit {digit}.
ident ::= letter {letter | digit}.
digit ;;= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9“.
letter ::= "a" | "b" | … | "y" | "z" | "A" | "B" | ... |"Y" | "Z".

 
Based on Wirth’s definition for EBNF we have the following rule:
[ ] means an optional item.
{ } means repeat 0 or more times.
Terminal symbols are enclosed in quote marks.
A period is used to indicate the end of the definition of a syntactic class.

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
`gcc -o parsercodegen parsercodegen.c`

Ensure input file is in the same directory then run the .exe passing the input file name:<br>
`./parsercodegen <inputFileName>`

## Example

```bash
# Example command and expected output
./parsercodegen input.txt
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
