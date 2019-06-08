#include "stdafx.h"

#ifndef _Structures_h
#define _Structures_h

#define MAX_INSTRUCTIONS 100
#define MAX_PARAMETERS 10000
#define MAX_MEMORY 100

struct symbol_table{
	char symbol;
	unsigned int address;
	int size;
};

struct label_table{
	char label;
	int instruction_number;
};

struct stack{
	int instruction_numbers[MAX_INSTRUCTIONS];
	int top;
};

struct Intermediate_language{
	int opcode;
	int parameters[MAX_PARAMETERS];
};

struct All_tables{
	struct symbol_table *Sym_Table[MAX_INSTRUCTIONS];
	struct label_table *label_Table[MAX_INSTRUCTIONS];
	struct stack *stk;
	struct Intermediate_language *Intermediate_code[MAX_INSTRUCTIONS];
	char memory[MAX_MEMORY];
	int symIndex;
	int labelIndex;
	int intermediateIndex;
	int memoryIndex;
};


#endif