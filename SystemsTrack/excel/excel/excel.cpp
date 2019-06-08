// excel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#define MAX_EXPRESSIONS 10
#define SHEET_SIZE 10
void inputDecode(struct cell **sheet, char *cmd, char *activeFile);
struct tree{
	int value;
	char operation;
	struct tree *left;
	struct tree *right;
};
struct cell{
	struct tree *formula;
	char *strFormula;
};

struct cell **createSheet(){
	struct cell **sheet;
	sheet = (struct cell **)malloc(sizeof(struct cell *)*SHEET_SIZE);
	for (int iter = 0; iter < SHEET_SIZE; iter++)
		sheet[iter] = (struct cell *)malloc(sizeof(struct cell)*SHEET_SIZE);
	for (int row = 0; row < SHEET_SIZE; row++)
		for (int col = 0; col < SHEET_SIZE; col++){
			sheet[row][col].formula = (struct tree *)malloc(sizeof(struct tree));
			sheet[row][col].formula->value = 0;
			sheet[row][col].formula->operation = '0';
			sheet[row][col].formula->left = NULL;
			sheet[row][col].formula->right = NULL;
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

int getCol(char ch){
	if (ch >= 'a'&&ch <= 'z')
		return ch - 'a';
	return ch - 'A';
}

int getRow(char ch){
	return ch - '1';
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
		row = getRow(cmd[iter1+1]);
	else
		row = 9;
	int val = evaluateTree(sheet[row][col].formula);
	printf("%d\n", val);

}

struct tree *createTreeNode1(char ch){
	struct tree *curr;
	curr = (struct tree *)malloc(sizeof(struct tree));
	curr->value = 0;
	curr->operation = ch;
	curr->right = NULL;
	curr->left = NULL;
	return curr;
}
struct tree *createTreeNode2(int val){
	struct tree *curr;
	curr = (struct tree *)malloc(sizeof(struct tree));
	curr->value = val;
	curr->operation = '0';
	curr->right = NULL;
	curr->left = NULL;
	return curr;
}

void buildFormula(struct cell **sheet,int iter,char *cmd,int x,int y){
	
	struct tree *root = NULL;
	int i;
	char *e1, *e2;
	int index_e1 = 0, index_e2 = 0;
	int value_e1 = 0, value_e2 = 0;
	int row=-1, col=-1;
	e1 = (char *)malloc(sizeof(char) * 5);
	e2 = (char *)malloc(sizeof(char) * 5);
	for (i = iter; cmd[i]; i++){
		if (cmd[i] == '+' || cmd[i] == '-' || cmd[i] == '*' || cmd[i] == '/'){
			sheet[x][y].formula->operation = cmd[i];
			sheet[x][y].formula->value = 0;
			root = sheet[x][y].formula;
			break;
		}
		e1[index_e1++] = cmd[i];
	}
	e1[index_e1] = '\0';
	if (e1[0] >= '0'&&e1[0] <= '9'){
		for (int j = 0; e1[j]; j++)
			value_e1 = (value_e1 * 10) + (e1[j] - '0');
	}
	else{
		col = e1[0] - 'A';
		if (strLength(e1) == 3)
			row = 9;
		else
			row = e1[1] - '1';
	}
	if (row == -1 && col == -1 && root == NULL){
		printf("%d %d\n", x, y);
		sheet[x][y].formula->value = value_e1;
		sheet[x][y].formula->operation = '0';
		sheet[x][y].formula->left=NULL;
		sheet[x][y].formula->right=NULL;
		return;

	}
	strcpy(sheet[x][y].strFormula, cmd);
	if (root == NULL){
		printf("%d %d %d %d\n", x, y, row, col);
		/*sheet[x][y].formula->value=sheet[row][col].formula->value;
		sheet[x][y].formula->operation = sheet[row][col].formula->operation;
		sheet[x][y].formula->left = sheet[row][col].formula->left;
		sheet[x][y].formula->right = sheet[row][col].formula->right;*/
		sheet[x][y].formula = sheet[row][col].formula;
		return;
	}

	//printf("Hello");

	if (row == -1 && col == -1)
		root->left = createTreeNode2(value_e1);
	else
		root->left = sheet[row][col].formula;

	for (int j = i+1; cmd[j]; j++)
		e2[index_e2++] = cmd[j];
	e2[index_e2] = '\0';


	row = -1;
	col = -1;

	if (e2[0] >= '0'&&e2[0] <= '9'){
		for (int j = 0; e2[j]; j++)
			value_e2 = (value_e2 * 10) + (e2[j] - '0');
	}
	else{
		col = e2[0] - 'A';
		if (strLength(e2) == 3)
			row = 9;
		else
			row = e2[1] - '1';
	}

	if (row == -1 && col == -1)
		root->right = createTreeNode2(value_e2);
	else
		root->right = sheet[row][col].formula;

	

}

int loopDetection(struct cell **sheet, int row,int col, char *cmd){
	if (cmd[0] == '-')
		return 1;
	int i;
	for (i = 0; cmd[i]; i++){
		if (cmd[i] == '='){
			i++;
			break;
		}
	}
	int r = -1, c = -1;
	char *cell;
	cell = (char *)malloc(sizeof(char) * 4);
	int indexCell = 0;
	for (int j = i; cmd[j]; j++){
		if ((cmd[j] >= 'A'&&cmd[j] <= 'Z') || (cmd[j] >= '1'&&cmd[j] <= '9')){
			cell[indexCell++] = cmd[j];
		}
		else{
			cell[indexCell] = '\0';
			if (!(cell[0] >= '0'&&cell[0] <= '9')){
				c = getCol(cell[0]);
				if (strLength(cell) == 3)
					r = 9;
				else
					r = getRow(cell[1]);
				if (r == row&&c == col)
					return 0;
				int res = loopDetection(sheet, row, col , sheet[r][c].strFormula);
				if (res == 0)
					return res;
			}
			indexCell = 0;

		}

	}
	cell[indexCell] = '\0';
	if (!(cell[0] >= '0'&&cell[0] <= '9')){
		c = getCol(cell[0]);
		if (cell[2]=='0')
			r = 9;
		else
			r = getRow(cell[1]);
		if (r == row&&c == col)
			return 0;
		int res = loopDetection(sheet, row, col, sheet[r][c].strFormula);
		if (res == 0)
			return res;
	}
	return 1;

}


void SET(struct cell **sheet, int iter, char *cmd){
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
	int loop = loopDetection(sheet, row, col, cmd);
	if (loop == 0){
		printf("Loop dependecy exist >_> invalid formula !!\n");
		return;
	}
	if (row==9)
		buildFormula(sheet, iter1 + 4, cmd,row,col);
	else
		buildFormula(sheet, iter1 + 3, cmd, row, col);
	//printf("%d %d %d\n",row,col, sheet[row][col].formula->value);
	return;
		
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
		printf("%*c|", 5, 'A'+col);
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
		printf("%*d|", 5, row+1);
		for (int col = 0; col < SHEET_SIZE; col++){
			int val = evaluateTree(sheet[row][col].formula);
			printf("%*d|", 5, val);
		}
		if (row != SHEET_SIZE - 1)
			printf("\n|");
		else
			printf("\n+");
		for (int line = 1; line < SHEET_SIZE * 6 + 6; line++){
			if (line % 6 == 0 && row!=SHEET_SIZE-1)
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

char *IMPORT(struct cell **sheet, int iter, char *cmd,char *fle){

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

			inputDecode(sheet, str,fle);

		}
		
	}
	fclose(fptr);
	return fle;
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

char *changeDependencies(struct cell **sheet, int row, int col){
	char *str;
	str = (char *)malloc(5);
	int index = 0;
	for (int i = 0; i < SHEET_SIZE; i++){
		if (col == i)
			continue;
		if (sheet[row][i].formula == sheet[row][col].formula->left){
			str[index++] = col + 'A';
			if (row == 9){
				str[index++] = '1';
				str[index++] = '0';
			}
			else
				str[index++] = row + '1';
			str[index] = '\0';
			return str;
		}
	}
}

void rowSort(struct cell **sheet, int row){
	for (int i = 0; i < SHEET_SIZE; i++){
		for (int j = 0; j < SHEET_SIZE - i - 1; j++){

			if (evaluateTree(sheet[row][j].formula) > evaluateTree(sheet[row][j+1].formula)){
				char *ch;
				ch = (char *)malloc(100);
				strcpy(ch, sheet[row][j].strFormula);
				strcpy(sheet[row][j].strFormula,sheet[row][j+1].strFormula);
				strcpy(sheet[row][j+1].strFormula, ch);

				struct tree *temp;
				temp = sheet[row][j].formula;
				sheet[row][j].formula = sheet[row][j+1].formula;
				sheet[row][j + 1].formula = temp;

			}
		}
	}
	printf("%s\n", sheet[0][9].strFormula);
	/*for (int i = 0; i < SHEET_SIZE; i++){


		if (sheet[row][i].strFormula[0] == '-')
			continue;
		char *str;
		str = (char *)malloc(100);
		int index = 5;
		str[0] = 'S';
		str[1] = 'E';
		str[2] = 'T';
		str[3] = ' ';
		str[4] = row + '1';
		if (i == 9){
			str[5] = '1';
			str[6] = '0';
			index += 2;
		}
		else{
			str[5] = i + 'A';
			index++;
		}
		str[index] = '\0';
		int j = 0;
		for (; sheet[row][i].strFormula[j];j++)
			if (sheet[row][i].strFormula[j] == ' '){
				j++;
				break;
			}
		char *cell;
		cell = (char *)malloc(5);
		int cellInd = 0;
		for (; sheet[row][i].strFormula[j]; j++){
			if (sheet[row][i].strFormula[j] == '+' || sheet[row][i].strFormula[j] == '-' || sheet[row][i].strFormula[j] == '*' || sheet[row][i].strFormula[j] == '/'){
				cell[cellInd] = '\0';
				if (cell[0] >= '1'&&cell[0] <= '9'){
					strcat(str, cell);
					index = index + strlen(cell);
					str[index++] = sheet[row][i].strFormula[j];
					str[index] = '\0';
					cellInd = 0;
					continue;
				}
				else{
					int col1 = -1;
					int row1 = -1;
					if (strLength(cell) == 3)
						row1 = 9;
					else
						row1 = getRow(cell[1]);
					if (row1 != row){
						strcat(str, cell);
						index = index + strlen(cell);
						str[index++] = sheet[row][i].strFormula[j];
						str[index] = '\0';
						cellInd = 0;
						continue;
					}
					else{
						char *res = changeDependencies(sheet, row, i);
						strcat(str, res);
						index = index + strlen(cell);
						str[index++] = sheet[row][i].strFormula[j];
						str[index] = '\0';
						cellInd = 0;
						continue;
					}
				}
			}
			cell[cellInd++] = sheet[row][i].strFormula[j];
		}
		cell[cellInd] = '\0';
		if (cell[0] >= '1'&&cell[0] <= '9'){
			strcat(str, cell);
			cellInd = 0;
			
		}
		else{
			int col1 = -1;
			int row1 = -1;
			if (strLength(cell) == 3)
				row1 = 9;
			else
				row1 = getRow(cell[1]);
			if (row1 != row){
				strcat(str, cell);
				cellInd = 0;
			}
			else{
				char *res = changeDependencies(sheet, row, i);
				strcat(str, res);
				cellInd = 0;
			}
		}
		str[index + strlen(cell)] = '\0';
		printf("%d\n", str);

	}

	*/


}

void colSort(struct cell **sheet, int col){

}

void SORT(struct cell **sheet, int iter, char *cmd){
	char rc;
	for (int i = iter; cmd[i]; i++){
		if (cmd[i] == ' ')
			continue;
		rc = cmd[i];
	}
	if (rc >= '1'&&rc <= '9'){
		int row = rc - '1';
		rowSort(sheet, row);
	}
	else{
		int col = rc - 'A';
		colSort(sheet, col);
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
	if (operation == 1)
		GET(sheet, iter, cmd);
	else if (operation == 2)
		SET(sheet, iter, cmd);
	else if (operation == 3)
		PRINT(sheet);
	else if (operation == 4){
		IMPORT(sheet, iter, cmd,activeFile);
	}
	else if (operation == 5)
		EXPORT(sheet, iter, cmd);
	else if (operation == 6)
		SAVE(sheet, activeFile);
	else if (operation == 7)
		exit(0);
	else if (operation == 8)
		SORT(sheet, iter, cmd);
	else if (cmd[0]!='-'&&(strLength(cmd)!=0))
		printf("Invalid Command\n");
	

}


void userInterface(){

	struct cell **sheet = createSheet();
	char *cmd = (char *)malloc(100);
	char *activeFile;
	activeFile = (char *)malloc(20);
	strcpy(activeFile, "\0");
 	while(1){
		printf("> ");
		gets(cmd);
		inputDecode(sheet, cmd,activeFile);
	}

}
/*
int _tmain(int argc, _TCHAR* argv[])
{
	userInterface();
	return 0;
}*/