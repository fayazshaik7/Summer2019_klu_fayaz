// excel1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"



struct cell **createSheet(){
	struct cell **sheet;
	sheet = (struct cell **)malloc(sizeof(struct cell *)*SHEET_SIZE);
	for (int iter = 0; iter < SHEET_SIZE; iter++)
		sheet[iter] = (struct cell *)malloc(sizeof(struct cell)*SHEET_SIZE);
	for (int row = 0; row < SHEET_SIZE; row++)
		for (int col = 0; col < SHEET_SIZE; col++){
			sheet[row][col].formula = (struct tree *)malloc(sizeof(struct tree));
			sheet[row][col].formula->value = 0;
			sheet[row][col].formula->operation = '=';
			sheet[row][col].formula->left = NULL;
			sheet[row][col].formula->right = NULL;
			sheet[row][col].formula->name = (char *)malloc(100);
			strcpy(sheet[row][col].formula->name, "\0");
			sheet[row][col].strFormula = (char *)malloc(sizeof(char) * 100);
			strcpy(sheet[row][col].strFormula, "-");
		}
	return sheet;
}
char *toUpper(char *a){
	for (int iter = 0; a[iter]; iter++){
		if (a[iter] >= 'a'&&a[iter] <= 'z'){
			a[iter] = a[iter] - 32;
		}
	}
	return a;
}

int strLength(char *a){
	int iter;
	for (iter = 0; a[iter]; iter++);
	return iter;
}

int strCompare(char *a, char *b){
	if (strLength(a) != strLength(b))
		return 0;
	for (int iter = 0; a[iter] && b[iter]; iter++){
		if (a[iter] != b[iter])
			return 0;
	}
	return 1;
}
int calculate(char ch, int a, int b){

	if (ch == '+')
		return a + b;
	if (ch == '-')
		return a - b;
	if (ch == '*')
		return a * b;
	if (ch == '/')
		return b / a;
	if (ch == '%')
		return b % a;
	return -1;
}
int evaluateTree(struct tree *root){
	if (root == NULL)
		return 0;
	if (root->operation == '=')
		return evaluateTree(root->left);
	if (root->left == NULL&&root->right == NULL)
		return root->value;
	int left = evaluateTree(root->left);
	int right = evaluateTree(root->right);
	return calculate(root->operation, left, right);
}
int getOperation(char *token){
	if (strCompare(toUpper(token), "GET\0"))
		return 1;
	if (strCompare(toUpper(token), "SET\0"))
		return 2;
	if (strCompare(toUpper(token), "PRINT\0"))
		return 3;
	if (strCompare(toUpper(token), "IMPORT\0"))
		return 4;
	if (strCompare(toUpper(token), "EXPORT\0"))
		return 5;
	if (strCompare(toUpper(token), "SAVE\0"))
		return 6;
	if (strCompare(toUpper(token), "EXIT\0"))
		return 7;
	if (strCompare(toUpper(token), "SORT\0"))
		return 8;
	if (strCompare(toUpper(token), "IMPORT_STUDENTS\0"))
		return 9;
	if (strCompare(toUpper(token), "SUM\0"))
		return 10;
	if (strCompare(toUpper(token), "AVG\0"))
		return 11;
	if (strCompare(toUpper(token), "MAX\0"))
		return 12;
	if (strCompare(toUpper(token), "MIN\0"))
		return 13;
	if (strCompare(toUpper(token), "TOTAL\0"))
		return 14;
	if (strCompare(toUpper(token), "TOPPER\0"))
		return 15;
	return -1;
}


int getCol(char ch){
	if (ch >= 'a'&&ch <= 'z')
		return ch - 'a';
	return ch - 'A';
}

int getRow(char ch){
	return ch - '1';
}
void GET(struct cell **sheet, int iter, char *cmd){
	int iter1;
	int row, col;
	for (iter1 = iter; cmd[iter1]; iter1++){
		if (cmd[iter1] == ' ')
			continue;
		else
			col = getCol(cmd[iter1]);
		break;
	}
	if (cmd[iter1 + 2] != '0')
		row = getRow(cmd[iter1 + 1]);
	else
		row = 9;
	int val = evaluateTree(sheet[row][col].formula);
	printf("%d\n", val);

}


void PRINT(struct cell **sheet){
	printf("\t\t\t***Sheet***\n");
	printf("\n+");
	for (int line = 1; line < SHEET_SIZE * 12 + 12; line++){
		printf("-");
	}
	printf("+\n|");
	printf("%*c|", 11, ' ');
	for (int col = 0; col < SHEET_SIZE; col++){
		printf("%*c|", 11, 'A' + col);
	}
	printf("\n|");
	for (int line = 1; line < SHEET_SIZE * 12 + 12; line++){
		if (line % 12 == 0)
			printf("+");
		else
			printf("-");
	}
	printf("|\n");
	for (int row = 0; row < SHEET_SIZE; row++){
		printf("|");
		printf("%*d|", 11, row + 1);
		for (int col = 0; col < SHEET_SIZE; col++){
			if (strlen(sheet[row][col].formula->name)>0)
				printf("%*s|", 11, sheet[row][col].formula->name);
			else{
				int val = sheet[row][col].formula->value;
				printf("%*d|", 11, val);
			}
		}
		if (row != SHEET_SIZE - 1)
			printf("\n|");
		else
			printf("\n+");
		for (int line = 1; line < SHEET_SIZE * 12 + 12; line++){
			if (line % 12 == 0 && row != SHEET_SIZE - 1)
				printf("+");
			else
				printf("-");
		}
		if (row != SHEET_SIZE - 1)
			printf("|\n");
		else
			printf("+\n");
	}
}

void inputDecode(struct cell **sheet, char *cmd, char *activeFile){
	char *token = (char *)malloc(7);
	int tokenIndex = 0;
	int operation = -1;
	int iter;
	for (iter = 0; cmd[iter]; iter++){
		if (cmd[iter] == ' '){
			break;
		}
		token[tokenIndex++] = cmd[iter];
	}
	token[tokenIndex] = '\0';
	operation = getOperation(token);
	if (operation == 1)
	GET(sheet, iter, cmd);
	else if (operation == 2)
	SET(sheet, iter, cmd);
	else if (operation == 3)
		PRINT(sheet);
	else if (operation == 4){
		IMPORT(sheet, iter, cmd, activeFile);
	}
	else if (operation == 5)
		EXPORT(sheet, iter, cmd);
	else if (operation == 6)
		SAVE(sheet, activeFile);
	else if (operation == 7)
		exit(0);
	else if (operation == 8);
	//SORT(sheet, iter, cmd);
	else if (operation == 9)
		IMPORT_STUDENTS(sheet, iter, cmd, activeFile);
	else if (operation == 10)
		SUM(sheet, iter, cmd);
	else if (operation == 11)
		AVG(sheet, iter, cmd);
	else if (operation == 12)
		MAX(sheet, iter, cmd);
	else if (operation == 13)
		MIN(sheet, iter, cmd);
	else if (operation == 14)
		TOTAL(sheet, iter, cmd);
	else if (operation == 15)
		TOPPER(sheet);
	else if (cmd[0] != '-' && (strLength(cmd) != 0))
		printf("Invalid Command\n");
}

void userInterface(){

	struct cell **sheet = createSheet();
	char *cmd = (char *)malloc(100);
	char *activeFile;
	activeFile = (char *)malloc(20);
	strcpy(activeFile, "\0");
	while (1){
		printf("> ");
		gets(cmd);
		inputDecode(sheet, cmd, activeFile);
	}

}

int main(){
	userInterface();
}