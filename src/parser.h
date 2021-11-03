#ifndef _PARSER_H_
#define _PARSER_H_

#include "number.h"
#include "values.h"
#include <stdbool.h>

typedef enum{
    OPERATION_PLUS, OPERATION_MINUS, OPERATION_MULT, OPERATION_DIV, OPERATION_CONST_LITERAL, OPERATION_CONST_IDENTIFIER
}Operation;

typedef struct tagExpression
{
    Operation operation;
    struct tagExpression *parameter0;
    struct tagExpression *parameter1;
    char identifier[VALUES_IDENTIFIER_CAPACITY];
    Number value;
}Expression;

typedef struct tagVariableDeclaration{
    Expression *expression;
    char identifier[VALUES_IDENTIFIER_CAPACITY];
    struct tagVariableDeclaration *next;
}VariableDeclaration;

typedef struct{
    VariableDeclaration *variables;
    Expression *expression;
}Program;

Program Parser_Analyze();

void Parser_PrintVariables(VariableDeclaration *variables);

void Parser_PrintExpression(Expression *expression);

void Parser_DestroyProgram(Program program);

void Parser_ExecuteProgram(Program program);

#endif
