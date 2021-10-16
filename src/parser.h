#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdbool.h>

typedef enum{
    OPERATION_PLUS, OPERATION_MINUS, OPERATION_MULT, OPERATION_DIV, OPERATION_CONST
}Operation;

typedef enum{
    EXPRESSION_TYPE_INT, EXPRESSION_TYPE_FLOAT
}ExpressionType;

typedef struct {
    bool isFloat;
    union{
        long valueInt;
        double valueFloat;
    }data;
}Number;

typedef struct tagExpression
{
    ExpressionType expressionType;
    Operation operation;

    struct tagExpression *parameter0;
    struct tagExpression *parameter1;
    
    Number value;

}Expression;


typedef struct{
    Expression *expression;
}Program;

Program Parser_Analyze();

void Parser_PrintExpression();

void Parser_DestroyProgram(Program program);

void Parser_ExecuteProgram(Program program);

#endif
