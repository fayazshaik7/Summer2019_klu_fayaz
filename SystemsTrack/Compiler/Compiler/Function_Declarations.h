#include "stdafx.h"

#ifndef _Function_Declarations_h
#define _Function_Declarations_h

void UserInterface();
void Generate_Intermediate_Code(char *fileName);
void tokenize(char *tokens, struct All_tables *all_tables);
void GetFunc(char *token, char **args, struct All_tables *all_tables);
void MOV(char *args[], struct All_tables *all_tables);
void DATA(char *arg, struct All_tables *all_tables);
void Add_to_symbol_table(char symbol, int size, struct All_tables *all_tables);
void CONSTANT(char *args[], struct All_tables *all_tables);
void ADD(char *args[], struct All_tables *all_tables);
void SUB(char *args[], struct All_tables *all_tables);
void MUL(char *args[], struct All_tables *all_tables);
void PRINT(char *arg, struct All_tables *all_tables);
int get_register_address(char *arg);
int get_variable_address(char ch, struct All_tables *all_tables);
void READ(char *arg, struct All_tables *all_tables);
void IF(char *args[], struct All_tables *all_tables);
void ELSE(struct All_tables *all_tables);
void ENDIF(struct All_tables *all_tables);
int pop_stack(struct stack *stk);
void Push_stack(struct stack *stk, int insNumber);
void write_to_file(struct All_tables *all_tables);
void Update_Block_table(char label, struct All_tables *all_tables);
void JUMP(char label, struct All_tables *all_tables);

#endif