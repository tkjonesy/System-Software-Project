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

example goes here

## Team Members

Team Member 1: Trever Jones, tr755969@ucf.edu<br>
Team Member 2: Jose Porta, Jose.porta@ucf.edu<br>

## Contact Information

For any questions or issues, please contact:

Name: Trever Jones<br>
Email: tr755969@ucf.edu
