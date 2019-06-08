#include "stdafx.h"


void Add_to_symbol_table(char symbol,int size, struct All_tables *all_tables){
	int newEntry = all_tables->symIndex;
	all_tables->Sym_Table[newEntry] = (struct symbol_table *)malloc(sizeof(struct symbol_table));
	all_tables->Sym_Table[newEntry]->symbol = symbol;
	all_tables->Sym_Table[newEntry]->address = all_tables->memoryIndex;
	all_tables->Sym_Table[newEntry]->size = size;
	//printf("%c %d %d\n", all_tables->Sym_Table[newEntry]->symbol, all_tables->Sym_Table[newEntry]->address, all_tables->Sym_Table[newEntry]->size);
	all_tables->memoryIndex += size;
	all_tables->symIndex++;
}

void Add_to_Intermediate_table(int opcode, int args[],struct All_tables *all_tables){
	int newEntry = all_tables->intermediateIndex;
	all_tables->Intermediate_code[newEntry] = (struct Intermediate_language *)malloc(sizeof(struct Intermediate_language));
	all_tables->Intermediate_code[newEntry]->opcode = opcode;
	all_tables->Intermediate_code[newEntry]->parameters[0] = args[0];
	all_tables->Intermediate_code[newEntry]->parameters[1] = args[1];
	all_tables->Intermediate_code[newEntry]->parameters[2] = args[2];
	all_tables->Intermediate_code[newEntry]->parameters[3] = args[3];
	all_tables->intermediateIndex++;
	//printf("%d %d %d %d %d\n", opcode, args[0], args[1], args[2], args[3]);
}


void DATA(char *arg, struct All_tables *all_tables){
	if (arg[1] != '[')
		Add_to_symbol_table(arg[0], 1, all_tables);
	else
		Add_to_symbol_table(arg[0], (arg[2] - '0'), all_tables);	
}

void CONSTANT(char *args[], struct All_tables *all_tables){
	all_tables->memory[all_tables->memoryIndex] = args[2][0];
	all_tables->memoryIndex++;
	Add_to_symbol_table(args[0][0], 0, all_tables);
}

int get_register_address(char *arg){
	return arg[0] - 'A';
}
int get_variable_address(char ch,struct All_tables *all_tables){
	for (int index = 0; index < all_tables->symIndex; index++){
		if (all_tables->Sym_Table[index]->symbol == ch)
			return all_tables->Sym_Table[index]->address;
	}
	return -1;
}

void READ(char *arg, struct All_tables *all_tables){
	int res = get_register_address(arg);
	int parms[4] = { 0 };
	parms[0] = res;
	parms[1] = -1;
	parms[2] = -1;
	parms[3] = -1;
	Add_to_Intermediate_table(14, parms, all_tables);
}

void MOV(char *args[], struct All_tables *all_tables){
	char var;
	int reg;
	if (args[0][1] == 'X'){
		var = args[1][0];
		reg = 0;
	}
	else{
		var = args[0][0];
		reg = 1;
	}
	int parms[4] = { 0 };
	parms[2] = -1;
	parms[3] = -1;
	if (reg == 0){
		parms[0] = get_register_address(args[reg]);
		parms[1] = get_variable_address(var,all_tables);
		Add_to_Intermediate_table(2, parms, all_tables);
	}
	else{
		parms[1] = get_register_address(args[reg]);
		parms[0] = get_variable_address(var,all_tables);
		Add_to_Intermediate_table(1, parms, all_tables);
	}
	
	
}

void ADD(char *args[], struct All_tables *all_tables){
	int parms[4];
	parms[0] = get_register_address(args[0]);
	parms[1] = get_register_address(args[1]);
	parms[2] = get_register_address(args[2]);
	parms[3] = -1;
	Add_to_Intermediate_table(3, parms, all_tables);
}

void SUB(char *args[], struct All_tables *all_tables){
	int parms[4];
	parms[0] = get_register_address(args[0]);
	parms[1] = get_register_address(args[1]);
	parms[2] = get_register_address(args[2]);
	parms[3] = -1;
	Add_to_Intermediate_table(4, parms, all_tables);
}

void MUL(char *args[], struct All_tables *all_tables){
	int parms[4];
	parms[0] = get_register_address(args[0]);
	parms[1] = get_register_address(args[1]);
	parms[2] = get_register_address(args[2]);
	parms[3] = -1;
	Add_to_Intermediate_table(5, parms, all_tables);
}
void PRINT(char *arg, struct All_tables *all_tables){
	int parms[4];
	parms[0] = get_variable_address(arg[0],all_tables);
	parms[1] = -1;
	parms[2] = -1;
	parms[3] = -1;
	Add_to_Intermediate_table(13, parms, all_tables);
}

int get_operator_index(char *arg){
	if (strcmp(arg, "EQ")==0)
		return 8;
	if (strcmp(arg, "LT")==0)
		return 9;
	if (strcmp(arg, "GT")==0)
		return 10;
	if (strcmp(arg, "LTEQ")==0)
		return 11;
	if (strcmp(arg ,"GTEQ")==0)
		return 12;
}


void Push_stack(struct stack *stk,int insNumber){
	stk->top++;
	stk->instruction_numbers[stk->top] = insNumber;
}

int pop_stack(struct stack *stk){
	int k = stk->instruction_numbers[stk->top--];
	return k;
}

void IF(char *args[], struct All_tables *all_tables){
	int parms[4];
	parms[0] = get_register_address(args[0]);
	parms[2] = get_operator_index(args[1]);
	parms[1] = get_register_address(args[2]);
	parms[3] = -2;
	Add_to_Intermediate_table(7, parms, all_tables);
	Push_stack(all_tables->stk, all_tables->intermediateIndex - 1);
}

void ELSE(struct All_tables *all_tables){
	int parms[4];
	parms[0] = -2;
	parms[2] = -1;
	parms[1] = -1;
	parms[3] = -1;
	Add_to_Intermediate_table(6, parms, all_tables);
	Push_stack(all_tables->stk, all_tables->intermediateIndex - 1);
}

void ENDIF(struct All_tables *all_tables){
	int res = all_tables->intermediateIndex;
	while (1){
		int curr_index = res;
		res = pop_stack(all_tables->stk);
		if (all_tables->Intermediate_code[res]->opcode == 6){
			all_tables->Intermediate_code[res]->parameters[0] = curr_index;
		}
		else if (all_tables->Intermediate_code[res]->opcode == 7){
			all_tables->Intermediate_code[res]->parameters[3] = curr_index;
			break;
		}
		else
			break;
		res++;
	}
}

void Update_Block_table(char label,struct All_tables *all_tables){
	all_tables->label_Table[all_tables->labelIndex] = (struct label_table *)malloc(sizeof(struct label_table));
	all_tables->label_Table[all_tables->labelIndex]->label = label;
	all_tables->label_Table[all_tables->labelIndex]->instruction_number = all_tables->intermediateIndex;
	all_tables->labelIndex++;
}

void JUMP(char label, struct All_tables *all_tables){
	int insIndex = -1;
	for (int index = 0; index < all_tables->labelIndex; index++){
		if (all_tables->label_Table[index]->label == label){
			insIndex = all_tables->label_Table[index]->instruction_number;
			break;
		}
	}
	if (insIndex == -1)
		return;
	int parms[4];
	parms[0] = insIndex;
	parms[2] = -1;
	parms[1] = -1;
	parms[3] = -1;
	Add_to_Intermediate_table(6, parms, all_tables);
}

void write_to_file(struct All_tables *all_tables){
	FILE *fp = fopen("output.txt", "w");
	for (int index = 0; index < all_tables->intermediateIndex; index++){
		fprintf(fp, "%d ", index);
		int args[5];
		int opcode = all_tables->Intermediate_code[index]->opcode;
		fprintf(fp, "%d ", opcode);
		args[0] = all_tables->Intermediate_code[index]->parameters[0];
		if (args[0] != -1)
			fprintf(fp, "%d ", args[0]);
		args[1] = all_tables->Intermediate_code[index]->parameters[1];
		if (args[1] != -1)
			fprintf(fp, "%d ", args[1]);
		args[2] = all_tables->Intermediate_code[index]->parameters[2];
		if (args[2] != -1)
			fprintf(fp, "%d ", args[2]);
		args[3] = all_tables->Intermediate_code[index]->parameters[3];
		if (args[3] != -1)
			fprintf(fp, "%d ", args[3]);
		fprintf(fp, "\n");
	}
	fclose(fp);
}