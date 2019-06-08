#include "stdafx.h"

void SUM(struct cell **sheet, int iter, char *cmd){
	char *token;
	token = (char *)malloc(10);
	int indTok = 0;

	for (int i=iter; cmd[i]; i++){
		if (cmd[i] == ' ')
			continue;
		token[indTok++] = cmd[i];
	}
	token[indTok] = '\0';

	int col = -1;
	for (int i = 0; i < SHEET_SIZE; i++){
		if (strlen(sheet[0][i].formula->name) == 0)
			break;
		if (strCompare(sheet[0][i].formula->name, token)){
			col = i;
			break;
		}
	}
	if (col == -1)
		return;
	int sum = 0;
	for (int i = 1; i <= 6; i++)
		sum += sheet[i][col].formula->value;
	printf("%d\n", sum);
	return;
}

void AVG(struct cell **sheet, int iter, char *cmd){
	char *token;
	token = (char *)malloc(10);
	int indTok = 0;

	for (int i = iter; cmd[i]; i++){
		if (cmd[i] == ' ')
			continue;
		token[indTok++] = cmd[i];
	}
	token[indTok] = '\0';

	int col = -1;
	for (int i = 0; i < SHEET_SIZE; i++){
		if (strlen(sheet[0][i].formula->name) == 0)
			break;
		if (strCompare(sheet[0][i].formula->name, token)){
			col = i;
			break;
		}
	}
	if (col == -1)
		return;
	int sum = 0;
	for (int i = 1; i <= 6; i++)
		sum += sheet[i][col].formula->value;
	double avg = double(sum) / 6.0;
	printf("%lf\n", avg);
	return;
}

void MAX(struct cell **sheet, int iter, char *cmd){
	char *token;
	token = (char *)malloc(10);
	int indTok = 0;

	for (int i = iter; cmd[i]; i++){
		if (cmd[i] == ' ')
			continue;
		token[indTok++] = cmd[i];
	}
	token[indTok] = '\0';

	int col = -1;
	for (int i = 0; i < SHEET_SIZE; i++){
		if (strlen(sheet[0][i].formula->name) == 0)
			break;
		if (strCompare(sheet[0][i].formula->name, token)){
			col = i;
			break;
		}
	}
	if (col == -1)
		return;
	int max = 0;
	char *name;
	name = (char *)malloc(15);
	for (int i = 1; i <= 6; i++){
		if (i == 1){
			max = sheet[i][col].formula->value;
			strcpy(name, sheet[i][0].formula->name);
		}
		else if (max < sheet[i][col].formula->value){
			max = sheet[i][col].formula->value;
			strcpy(name, sheet[i][0].formula->name);
		}
	}
	printf("%s %d\n",name, max);
	return;
}

void MIN(struct cell **sheet, int iter, char *cmd){
	char *token;
	token = (char *)malloc(10);
	int indTok = 0;

	for (int i = iter; cmd[i]; i++){
		if (cmd[i] == ' ')
			continue;
		token[indTok++] = cmd[i];
	}
	token[indTok] = '\0';

	int col = -1;
	for (int i = 0; i < SHEET_SIZE; i++){
		if (strlen(sheet[0][i].formula->name) == 0)
			break;
		if (strCompare(sheet[0][i].formula->name, token)){
			col = i;
			break;
		}
	}
	if (col == -1)
		return;
	int min = 0;
	char *name;
	name = (char *)malloc(15);
	for (int i = 1; i <= 6; i++){
		if (i == 1){
			min = sheet[i][col].formula->value;
			strcpy(name,sheet[i][0].formula->name);
		}
		else if (min > sheet[i][col].formula->value){
			min = sheet[i][col].formula->value;
			strcpy(name,sheet[i][0].formula->name);
		}
	}
	printf("%s %d\n",name, min);
	return;
}

void TOTAL(struct cell **sheet, int iter, char *cmd){
	char *token;
	token = (char *)malloc(10);
	int indTok = 0;

	for (int i = iter; cmd[i]; i++){
		if (cmd[i] == ' ')
			continue;
		token[indTok++] = cmd[i];
	}
	token[indTok] = '\0';

	int row = -1;
	for (int i = 0; i < SHEET_SIZE; i++){
		if (strlen(sheet[i][0].formula->name) == 0)
			break;
		if (strCompare(sheet[i][0].formula->name, token)){
			row = i;
			break;
		}
	}
	if (row == -1){
		printf("Not Found!!\n");
		return;
	}
	int sum = 0;
	for (int i = 1; i <= 6; i++)
		sum += sheet[row][i].formula->value;
	printf("%s Total = %d\n",sheet[row][0].formula->name ,sum);
	return;
}


void TOPPER(struct cell **sheet){
	int maxTotal = 0;
	char *name;
	name = (char *)malloc(15);
	for (int i = 0; i < 7; i++){
		int sum = 0;
		for (int j = 0; j < 7; j++){
			sum += sheet[i][j].formula->value;
		}
		if (i == 0){
			maxTotal = sum;
			strcpy(name, sheet[i][0].formula->name);
		}
		else if (sum > maxTotal){
			maxTotal = sum;
			strcpy(name, sheet[i][0].formula->name);
		}
	}

	printf("Topper : %s | Marks : %d\n", name, maxTotal);
}