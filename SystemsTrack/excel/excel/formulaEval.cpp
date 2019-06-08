#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#define MAX_EXPRESSIONS 10
#define SHEET_SIZE 10

struct tree{
	int value;
	char operation;
	struct tree *left;
	struct tree *right;
};
struct node{
	struct tree *formula;
	int row;
	int col;
};
struct cell{
	struct node *tree;
};

struct cell **createSheet(){
	struct cell **sheet;
	sheet = (struct cell **)malloc(sizeof(struct cell *)*SHEET_SIZE);
	for (int iter = 0; iter < SHEET_SIZE; iter++)
		sheet[iter] = (struct cell *)malloc(sizeof(struct cell)*SHEET_SIZE);
	for (int row = 0; row < SHEET_SIZE; row++)
		for (int col = 0; col < SHEET_SIZE; col++){
			sheet[row][col].tree = (struct node *)malloc(sizeof(struct node));
			sheet[row][col].tree->formula = (struct tree *)malloc(sizeof(struct tree));
			sheet[row][col].tree->formula->value = 0;
			sheet[row][col].tree->formula->operation = '0';
			sheet[row][col].tree->formula->left = NULL;
			sheet[row][col].tree->formula->right = NULL;
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
		return a / b;
	return -1;
}
int evaluateTree(struct tree *root){
	if (root->left == NULL && root->right == NULL)
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
	return -1;
}

void PRINT(struct cell **sheet){
	printf("\t\t\t***Sheet***\n");
	printf("\n+");
	for (int line = 1; line < SHEET_SIZE * 6 + 6; line++){
		printf("-");
	}
	printf("+\n|");
	printf("%*c|", 5, ' ');
	for (int col = 0; col < SHEET_SIZE; col++){
		printf("%*c|", 5, 'A' + col);
	}
	printf("\n|");
	for (int line = 1; line < SHEET_SIZE * 6 + 6; line++){
		if (line % 6 == 0)
			printf("+");
		else
			printf("-");
	}
	printf("|\n");
	for (int row = 0; row < SHEET_SIZE; row++){
		printf("|");
		printf("%*d|", 5, row + 1);
		for (int col = 0; col < SHEET_SIZE; col++){
			int val = evaluateTree(sheet[row][col].tree->formula);
			printf("%*d|", 5, val);
		}
		if (row != SHEET_SIZE - 1)
			printf("\n|");
		else
			printf("\n+");
		for (int line = 1; line < SHEET_SIZE * 6 + 6; line++){
			if (line % 6 == 0 && row != SHEET_SIZE - 1)
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

void inputDecode(struct cell **sheet, char *cmd,char *activeFile){
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
	if (operation == 1);
		//GET(sheet, iter, cmd);
	else if (operation == 2);
		//SET(sheet, iter, cmd);
	else if (operation == 3)
		PRINT(sheet);
	else if (operation == 4){
		//IMPORT(sheet, iter, cmd, activeFile);
	}
	else if (operation == 5);
		//EXPORT(sheet, iter, cmd);
	else if (operation == 6);
	//SAVE(sheet, activeFile);
	else if (operation == 7)
		exit(0);
	else if (operation == 8);
		//SORT(sheet, iter, cmd);
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