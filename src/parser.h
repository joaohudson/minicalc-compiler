#ifndef _PARSER_H_
#define _PARSER_H_

typedef enum{
    OPERATION_PLUS, OPERATION_MINUS, OPERATION_MULT, OPERATION_DIV
}Operation;

typedef enum{
    PARAMETER_TYPE_INT, PARAMETER_TYPE_FLOAT
}ParameterType;

typedef struct 
{
    ParameterType parameterType;
    Operation operation;
    
    union{
        double valueF;
        long value;
    } parameter0;
    
    union 
    {
        double valueF;
        long value;
    } parameter1;

}Expression;


typedef struct{
    Expression *expression;
}Program;

Program Parser_Analyze();

void Parser_DestroyProgram(Program program);

void Parser_ExecuteProgram(Program program);

#endif
