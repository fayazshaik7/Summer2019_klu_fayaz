// Compiler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void Generate_Intermediate_Code(char *fileName){
	FILE *fp = fopen(fileName, "rb");
	if (fp == NULL){
		printf("Error opening file!!\n");
		return;
	}
	struct All_tables *all_tables;
	all_tables = (struct All_tables *)malloc(sizeof(struct All_tables));
	all_tables->symIndex = 0;
	all_tables->labelIndex = 0;
	all_tables->intermediateIndex = 0;
	all_tables->memoryIndex = 0;
	all_tables->stk = (struct stack *)malloc(sizeof(struct stack));
	all_tables->stk->top = -1;
	char *line;
	line = (char *)malloc(100);
	while (fscanf(fp,"%[^\n]s",line)!=EOF){
		tokenize(line, all_tables);
		char ch;
		fscanf(fp, "%c", &ch);
	}
	write_to_file(all_tables);

}

void UserInterface(){
	char *fileName;
	fileName = (char *)malloc(20);
	while (1){
		printf("> Enter file name : ");
		gets(fileName);
		Generate_Intermediate_Code(fileName);
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	UserInterface();
	return 0;
}

