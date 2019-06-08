#include "stdafx.h"

void IMPORT(struct cell **sheet, int iter, char *cmd, char *fle){

	int fileIndex = 0;
	int extension = 0;
	for (int iter1 = iter; cmd[iter1]; iter1++){
		if (cmd[iter1] == ' ')
			continue;
		fle[fileIndex++] = cmd[iter1];
		if (cmd[iter1] == '.')
			extension = 1;
	}
	if (extension == 0){
		fle[fileIndex++] = '.';
		fle[fileIndex++] = 'c';
		fle[fileIndex++] = 's';
		fle[fileIndex++] = 'v';
	}
	char ch;
	fle[fileIndex] = '\0';
	FILE *fptr;
	fptr = fopen(fle, "r");
	for (int row = 0; row < SHEET_SIZE; row++){
		for (int col = 0; col < SHEET_SIZE; col++){
			int val;
			char *str;
			str = (char *)malloc(sizeof(char) * 100);
			fscanf(fptr, "%d", &val);
			sheet[row][col].formula->value = val;
			fscanf(fptr, "%c", &ch);
			fscanf(fptr, "%[^\n]s", str);

			inputDecode(sheet, str, fle);

		}

	}
	fclose(fptr);
	
}

void EXPORT(struct cell **sheet, int iter, char *cmd){
	char *fle;
	fle = (char *)malloc(20);
	int fileIndex = 0;
	int extension = 0;
	for (int iter1 = iter; cmd[iter1]; iter1++){
		if (cmd[iter1] == ' ')
			continue;
		fle[fileIndex++] = cmd[iter1];
		if (cmd[iter1] == '.')
			extension = 1;
	}
	if (extension == 0){
		fle[fileIndex++] = '.';
		fle[fileIndex++] = 'c';
		fle[fileIndex++] = 's';
		fle[fileIndex++] = 'v';
	}
	fle[fileIndex] = '\0';
	FILE *fptr;
	fptr = fopen(fle, "w");
	for (int row = 0; row < SHEET_SIZE; row++){
		for (int col = 0; col < SHEET_SIZE; col++){
			int val = evaluateTree(sheet[row][col].formula);
			fprintf(fptr, "%d", val);
			fprintf(fptr, " %s", sheet[row][col].strFormula);
			fprintf(fptr, "\n");
		}
	}
	fclose(fptr);
}

void SAVE(struct cell **sheet, char *activeFile){
	if (strlen(activeFile) <= 1){
		printf("Enter file name to save : ");
		scanf("%s", activeFile);
	}
	FILE *fptr;
	fptr = fopen(activeFile, "w");
	for (int row = 0; row < SHEET_SIZE; row++){
		for (int col = 0; col < SHEET_SIZE; col++){
			int val = evaluateTree(sheet[row][col].formula);
			fprintf(fptr, "%d", val);
			fprintf(fptr, " %s", sheet[row][col].strFormula);
			fprintf(fptr, "\n");
		}
	}
	fclose(fptr);
}
int convertToInt(char *str){
	int val = 0;
	for (int i = 0; str[i]; i++)
		val = (val * 10) + (str[i] - '0');
	return val;
}

void splitInput(struct cell **sheet, char *str, int row, int col){
	char *inp;
	inp = (char *)malloc(sizeof(100));
	int inpInd = 0;
	
	for (int i = 0; str[i]; i++){
		if (str[i] == ','){
			inp[inpInd] = '\0';
			if (inp[0] >= '0'&&inp[0] <= '9'){
				int val = convertToInt(inp);
				//printf("%d\n", val);
				sheet[row][col].formula->value = val;
			}
			else
				strcpy(sheet[row][col].formula->name, inp);

			inpInd = 0;
			col++;
			continue;
		}
		inp[inpInd++] = str[i];
	}
}


void IMPORT_STUDENTS(struct cell **sheet, int iter, char *cmd, char *fle){
	int fileIndex = 0;
	int extension = 0;
	for (int iter1 = iter; cmd[iter1]; iter1++){
		if (cmd[iter1] == ' ')
			continue;
		fle[fileIndex++] = cmd[iter1];
		if (cmd[iter1] == '.')
			extension = 1;
	}
	if (extension == 0){
		fle[fileIndex++] = '.';
		fle[fileIndex++] = 'c';
		fle[fileIndex++] = 's';
		fle[fileIndex++] = 'v';
	}
	fle[fileIndex] = '\0';
	FILE *fptr;
	fptr = fopen(fle, "r");
	printf("%s\n", fle);
	for (int row = 0; row < 7; row++){
		char *str;
		str = (char *)malloc(100);
		fgets(str, 100,fptr);
		//printf("%s\n", str);
		splitInput(sheet, str, row, 0);
	}
	fclose(fptr);
}