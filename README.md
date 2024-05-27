# Homework 1: Virtual Machine Implementaion

## Description
This project is an implementaion of a Virtual Machine. A VM is a software implemenation of an instruction set architecture. This project is written in c and will run with an input file that contains an instruction set architecture. 

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
`gcc -o vm vm.c`

Ensure input file is in the same directory then run the .exe passing the input file name:<br>
`vm.exe <inputFileName>`

## Example

Provide a simple example to illustrate how to use your program:
```bash
# Example command and expected output
vm.exe input.txt
Output: Result of processing input.txt
				PC	BP	SP	stack
Initial values:	10	499	500

	JMP	0	45	45	499	500	
	INC	0	5	48	499	495	0 0 0 0 0 
Please Enter an Integer: 3
	SYS	0	2	51	499	494	0 0 0 0 0 3 
	CAL	0	6	6	493	494	0 0 0 0 0 3 
	INC	0	4	9	493	490	0 0 0 0 0 3 | 499 499 54 0 
	LIT	0	4	12	493	489	0 0 0 0 0 3 | 499 499 54 0 4 
	LIT	0	3	15	493	488	0 0 0 0 0 3 | 499 499 54 0 4 3 
	MUL	0	3	18	493	489	0 0 0 0 0 3 | 499 499 54 0 12 
	STO	1	4	21	493	490	0 0 0 0 12 3 | 499 499 54 0 
	LIT	0	14	24	493	489	0 0 0 0 12 3 | 499 499 54 0 14 
	LOD	1	4	27	493	488	0 0 0 0 12 3 | 499 499 54 0 14 12 
	LSS	0	7	30	493	489	0 0 0 0 12 3 | 499 499 54 0 0 
	JPC	0	39	39	493	490	0 0 0 0 12 3 | 499 499 54 0 
	LIT	0	5	42	493	489	0 0 0 0 12 3 | 499 499 54 0 5 
	RTN	0	0	54	499	494	0 0 0 0 12 3 
Output result is: 3
	SYS	0	1	57	499	495	0 0 0 0 12 
	SYS	0	3	60	499	495	0 0 0 0 12 
```

## Team Members

Team Member 1: Trever Jones, tr755969@ucf.edu
Team Member 2: Jose Porta, Jose.porta@ucf.edu

## Contact Information
For any questions or issues, please contact:

Name: Trever Jones
Email: tr755969@ucf.edu


