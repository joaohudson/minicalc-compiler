#include "parser.h"
#include "lexer.h"
#include "file.h"
#include <stdlib.h>
#include <stdbool.h>

static int TokenTypeToOperation(TokenType token){
    switch (token)
    {
        case TOKEN_SYMBOL_PLUS:
            return OPERATION_PLUS;

        case TOKEN_SYMBOL_MULT:
            return OPERATION_MULT;

        case TOKEN_SYMBOL_MINUS:
            return OPERATION_MINUS;

        case TOKEN_SYMBOL_DIV:
            return OPERATION_DIV;

        default:
            return -1;
    }
}

static Expression* ExpressionAnalyze(){
    Expression *expression = malloc(sizeof(Expression));
    expression->parameter0 = NULL;
    expression->parameter1 = NULL;

    Assert(expression != NULL, "[parser.c] - [Parser_Analyze] - Nao foi possivel alocar memoria!\n");

    Token token;

    token = Lexer_NextToken();
    Lexer_CheckToken(token);

    if(token.type == TOKEN_INT){
        expression->expressionType = EXPRESSION_TYPE_INT;
        expression->operation = OPERATION_CONST;
        expression->value.data.valueInt = token.data.valueInt;
        return expression;
    }

    if(token.type == TOKEN_FLOAT){
        expression->expressionType = EXPRESSION_TYPE_FLOAT;
        expression->operation = OPERATION_CONST;
        expression->value.data.valueFloat = token.data.valueFloat;
        return expression;
    }

    if(token.type != TOKEN_SYMBOL_OPENPAR){
        fprintf(stderr, "Erro de sintaxe: token esperado \'(\'\n");
        exit(1);
    }

    expression->parameter0 = ExpressionAnalyze();

    token = Lexer_NextToken();
    Lexer_CheckToken(token);

    int operation = TokenTypeToOperation(token.type);
    if(operation == -1){
        fprintf(stderr, "Erro de sintaxe: era esperado um operador\n");
        exit(1);
    }
    expression->operation = operation;

    expression->parameter1 = ExpressionAnalyze();

    token = Lexer_NextToken();
    Lexer_CheckToken(token);

    if(token.type != TOKEN_SYMBOL_CLOSEPAR){
        fprintf(stderr, "Erro de sintaxe: token esperado \')\'\n");
        exit(1);
    }

    //se qualquer uma dos parâmetros for do tipo float, então a expressão é do tipo float
    expression->expressionType = expression->parameter0->expressionType == EXPRESSION_TYPE_FLOAT
                                    ||
                                 expression->parameter1->expressionType == EXPRESSION_TYPE_FLOAT
                                 ? EXPRESSION_TYPE_FLOAT
                                 : EXPRESSION_TYPE_INT;

    return expression;
}

Program Parser_Analyze(){
    Program program;

    Token token;

    token = Lexer_NextToken();
    Lexer_CheckToken(token);

    if(token.type != TOKEN_PRINT){
        fprintf(stderr, "Erro de sintaxe: palavra chave \'print\' esperada no inicio da expressao\n");
        exit(1);
    }

    program.expression = ExpressionAnalyze();

    return program;
}

void Parser_PrintExpression(Expression *expression){
    if(expression->expressionType == EXPRESSION_TYPE_INT){
        if(expression->operation == OPERATION_CONST){
            printf("%ld\n", expression->value.data.valueInt);
            return;
        }
        Parser_PrintExpression(expression->parameter0);
        Parser_PrintExpression(expression->parameter1);
    }else{
        if(expression->operation == OPERATION_CONST){
            printf("%lf\n", expression->value.data.valueFloat);
            return;
        }
        Parser_PrintExpression(expression->parameter0);
        Parser_PrintExpression(expression->parameter1);
    }
}

static Number ComputeExpression(Expression *a){
    
    Number result, x, y;

    if(a->expressionType == EXPRESSION_TYPE_INT)
    {
        result.isFloat = false;
        switch (a->operation)
        {
        case OPERATION_CONST:
            result.data.valueInt = a->value.data.valueInt;
        break;

        case OPERATION_PLUS:
            x = ComputeExpression(a->parameter0);
            y = ComputeExpression(a->parameter1);
            if(x.isFloat || y.isFloat){
                result.isFloat = true;
                result.data.valueFloat = x.data.valueFloat + y.data.valueFloat;
            }
        break;

        case OPERATION_MINUS:
            x = ComputeExpression(a->parameter0);
            y = ComputeExpression(a->parameter1);
            if(x.isFloat || y.isFloat){
                result.isFloat = true;
                result.data.valueFloat = x.data.valueFloat - y.data.valueFloat;
            }
        break;

        case OPERATION_MULT:
            x = ComputeExpression(a->parameter0);
            y = ComputeExpression(a->parameter1);
            if(x.isFloat || y.isFloat){
                result.isFloat = true;
                result.data.valueFloat = x.data.valueFloat * y.data.valueFloat;
            }
        break;

        case OPERATION_DIV:
            x = ComputeExpression(a->parameter0);
            y = ComputeExpression(a->parameter1);
            if(x.isFloat || y.isFloat){
                result.isFloat = true;
                result.data.valueFloat = x.data.valueFloat / y.data.valueFloat;
            }
        break;
        }
    }else
    {
        result.isFloat = true;
        switch (a->operation)
        {
        case OPERATION_CONST:
            result.data.valueFloat = a->value.data.valueFloat;
        break;

        case OPERATION_PLUS:
            x = ComputeExpression(a->parameter0);
            y = ComputeExpression(a->parameter1);
            if(!x.isFloat && !y.isFloat){
                result.isFloat = false;
                result.data.valueInt = x.data.valueInt + y.data.valueInt;
            }
        break;

        case OPERATION_MINUS:
            x = ComputeExpression(a->parameter0);
            y = ComputeExpression(a->parameter1);
            if(!x.isFloat && !y.isFloat){
                result.isFloat = false;
                result.data.valueInt = x.data.valueInt - y.data.valueInt;
            }
        break;

        case OPERATION_MULT:
            x = ComputeExpression(a->parameter0);
            y = ComputeExpression(a->parameter1);
            if(!x.isFloat && !y.isFloat){
                result.isFloat = false;
                result.data.valueInt = x.data.valueInt * y.data.valueInt;
            }
        break;

        case OPERATION_DIV:
            x = ComputeExpression(a->parameter0);
            y = ComputeExpression(a->parameter1);
            if(!x.isFloat && !y.isFloat){
                result.isFloat = false;
                result.data.valueInt = x.data.valueInt / y.data.valueInt;
            }
        break;
        }
    }

    return result;
}

void Parser_ExecuteProgram(Program program){
    Expression *expression = program.expression;
    Number number = ComputeExpression(expression);
    if(number.isFloat)
        printf("%lf\n", number.data.valueFloat);
    else
        printf("%ld\n", number.data.valueInt);
}

static void DestroyExpression(Expression *expression){
    DestroyExpression(expression->parameter0);
    DestroyExpression(expression->parameter1);
    free(expression);
}

void Parser_DestroyProgram(Program program){
    DestroyExpression(program.expression);
}
