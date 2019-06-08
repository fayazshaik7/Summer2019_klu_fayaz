#include "stdafx.h"



void GetFunc(char *token,char *args[],struct All_tables *all_tables){
	if (strcmp(token, "MOV\0") == 0)
		MOV(args, all_tables);
	if (strcmp(token, "ADD\0") == 0)
		ADD(args, all_tables);
	if (strcmp(token, "SUB\0") == 0)
		SUB(args, all_tables);
	if (strcmp(token, "MUL\0") == 0)
		MUL(args, all_tables);
	if (strcmp(token, "JMP\0") == 0)
		JUMP(args[0][0], all_tables);
	if (strcmp(token, "IF\0") == 0)
		IF(args, all_tables);
	if (strcmp(token, "PRINT\0") == 0)
		PRINT(args[0], all_tables);
	if (strcmp(token, "READ") == 0)
		READ(args[0], all_tables);
	if (strcmp(token, "DATA\0") == 0)
		DATA(args[0], all_tables);
	if (strcmp(token, "CONST\0") == 0)
		CONSTANT(args, all_tables);
	if (strcmp(token, "ELSE\r") == 0)
		ELSE(all_tables);
	if (strcmp(token, "ENDIF\r") == 0)
		ENDIF(all_tables);
	if (strcmp(token, "START:") == 0)
		return;
	if (token[1] == ':')
		Update_Block_table(token[0],all_tables);
}


void tokenize(char *tokens,struct All_tables *all_tables){
	char token[10];
	int tokenIndex = 0;
	int index;
	int alpha = 0;
	for (index = 0; tokens[index]; index++){
		if (tokens[index] == ' '&&alpha==1){
			break;
		}
		if (tokens[index] == ' ')
			continue;
		alpha = 1;
		token[tokenIndex++] = tokens[index];
	}
	token[tokenIndex] = '\0';
	char *args[100];
	args[0] = (char *)malloc(sizeof(char)*100);
	int argIndex = 0;
	int argNumber = 0;
	int rIndex;
	for (rIndex = index + 1; tokens[rIndex]; rIndex++){
		if (tokens[rIndex] == ','||tokens[rIndex]==' '){
			args[argNumber][argIndex] = '\0';
			argNumber++;
			args[argNumber] = (char *)malloc(sizeof(char)*100);
			argIndex = 0;
			if (tokens[rIndex + 1] == ' ')
				rIndex++;
			continue;
		}
		args[argNumber][argIndex++] = tokens[rIndex];
	}
	args[argNumber][argIndex] = '\0';
	argNumber++;
	GetFunc(token, args, all_tables);

}
