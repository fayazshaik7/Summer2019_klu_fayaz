// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <string.h>
#ifndef _stdfax_h
#define _stdfax_h

#define MAX_EXPRESSIONS 10
#define SHEET_SIZE 10


struct tree{
	char *name;
	int value;
	char operation;
	struct tree *left;
	struct tree *right;
};
struct cell{
	struct tree *formula;
	char *strFormula;
};
void PRINT(struct cell **);
struct cell **createSheet();
char *toUpper(char *);
int strLength(char *);
int strCompare(char *, char *);
int calculate(char , int , int );
int evaluateTree(struct tree *);
int getOperation(char *);
void PRINT(struct cell **);
void inputDecode(struct cell **, char *, char *);
int getCol(char);
int getRow(char);
void SET(struct cell **, int , char *);
void buildFormula(struct cell **, int , char *, int , int );
int loopDetection(struct cell **, int , int , char *);
struct tree *createTreeNode1(char );
struct tree *createTreeNode2(int );
void postfixEvaluation(struct cell **sheet, char *postfix[], int resIndex, int x, int y, char *cmd);
struct tree *createNode(char);
struct tree *createNode1(int);
void IMPORT(struct cell **sheet, int iter, char *cmd, char *fle);
void EXPORT(struct cell **sheet, int iter, char *cmd);
void SAVE(struct cell **sheet, char *activeFile);
void splitInput(struct cell **sheet, char *str, int row, int col);
int convertToInt(char *);
void IMPORT_STUDENTS(struct cell **sheet, int iter, char *cmd, char *fle);
void SUM(struct cell **sheet, int iter, char *cmd);
void AVG(struct cell **sheet, int iter, char *cmd);
void MAX(struct cell **sheet, int iter, char *cmd);
void MIN(struct cell **sheet, int iter, char *cmd);
void TOTAL(struct cell **sheet, int iter, char *cmd);
void TOPPER(struct cell **sheet);
#endif



// TODO: reference additional headers your program requires here
