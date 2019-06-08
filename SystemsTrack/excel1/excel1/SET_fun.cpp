#include "stdafx.h"

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
	if (row == 9)
		buildFormula(sheet, iter1 + 4, cmd, row, col);
	else
		buildFormula(sheet, iter1 + 3, cmd, row, col);
	//printf("%d %d %d\n",row,col, sheet[row][col].formula->value);
	return;

}


int getPriority(char ch){
	if (ch == '^')
		return 3;
	else if (ch == '*' || ch == '/' || ch == '%')
		return 2;
	else if (ch == '+' || ch == '-')
		return 1;
	else
		return 0;
}


void buildFormula(struct cell **sheet, int iter, char *cmd, int x, int y){
	char *postfix[100];
	int resIndex = 0;
	for (int i = 0; i < 15;i++)
		postfix[i] = (char *)malloc(100);
	char *temp;
	temp = (char *)malloc(5);
	int tempInd = 0;
	char *stk;
	stk = (char *)malloc(100);
	int top = -1;

	for (int j = iter; cmd[j]; j++){
		if (cmd[j] == '+' || cmd[j] == '-' || cmd[j] == '/' || cmd[j] == '*' || cmd[j] == '^' || cmd[j] == '(' || cmd[j] == ')'||cmd[j]=='%'){
			temp[tempInd] = '\0';
			if (tempInd>0)
				strcpy(postfix[resIndex++], temp);

			if (cmd[j] == '('){
				stk[++top] = '(';
				continue;
			}
			tempInd = 0;

			if (cmd[j] == ')'){
				while (top != -1 && stk[top] != '('){
					
					temp[0] = stk[top];
					temp[1] = '\0';
					strcpy(postfix[resIndex++], temp);
					
					top--;
				}
				top--;
				continue;
			}

			while (top != -1 && (getPriority(stk[top]) > getPriority(cmd[j]))){
				
				temp[0] = stk[top];
				temp[1] = '\0';
				strcpy(postfix[resIndex++], temp);
				top--;
			}
			stk[++top] = cmd[j];
			continue;

		}
		temp[tempInd++] = cmd[j];
	}
	temp[tempInd] = '\0';
	if (tempInd>0)
		strcpy(postfix[resIndex++], temp);
	while (top != -1){
		temp[0] = stk[top];
		temp[1] = '\0';
		strcpy(postfix[resIndex++], temp);
		top--;
	}
	/*for (int i = 0; i < resIndex;i++)
		printf("%s\n", postfix[i]);*/
	postfixEvaluation(sheet, postfix,resIndex, x, y,cmd);
	
}

void postfixEvaluation(struct cell **sheet,char *postfix[],int resIndex, int x, int y,char *cmd){

	struct tree *stk[100];
	for (int i = 0; i < 50;i++)
		stk[i] = (struct tree *)malloc(sizeof(struct tree));
	int top = -1;

	char *cell;
	cell = (char *)malloc(5);
	int index = 0;


	for (int i = 0; i<resIndex; i++){
			
		cell = postfix[i];
			printf("%s\n", cell);

			if (cell[0] == '+' || cell[0] == '-' || cell[0] == '*' || cell[0] == '/' || cell[0] == '%'){
				struct tree *temp = createNode(cell[0]);
				temp->left = stk[top--];
				temp->right = stk[top--];
				stk[++top] = temp;
				index = 0;
				continue;
			}
			if (cell[0] >= '1'&&cell[0] <= '9'){
				int val = 0;
				for (int j = 0; cell[j]; j++)
					val = (val * 10) + (cell[j] - '0');
				stk[++top] = createNode1(val);
				index = 0;
				continue;
			}
			strcpy(sheet[x][y].strFormula, cmd);
			int col = getCol(cell[0]);
			int row = -1;
			if (index == 3)
				row = 9;
			else
				row = getRow(cell[1]);
			stk[++top] = sheet[row][col].formula;
			index = 0;
			continue;
	}

	sheet[x][y].formula->left = stk[top];

}



int loopDetection(struct cell **sheet, int row, int col, char *cmd){
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
				int res = loopDetection(sheet, row, col, sheet[r][c].strFormula);
				if (res == 0)
					return res;
			}
			indexCell = 0;

		}

	}
	cell[indexCell] = '\0';
	if (!(cell[0] >= '0'&&cell[0] <= '9')){
		c = getCol(cell[0]);
		if (cell[2] == '0')
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
struct tree *createNode1(int val){
	struct tree *curr;
	curr = (struct tree *)malloc(sizeof(struct tree));
	curr->value = val;
	curr->operation = '0';
	curr->left = NULL;
	curr->right = NULL;
	return curr;
}
struct tree *createNode(char val){
	struct tree *curr;
	curr = (struct tree *)malloc(sizeof(struct tree));
	curr->value = -1;
	curr->operation = val;
	curr->left = NULL;
	curr->right = NULL;
	return curr;
}