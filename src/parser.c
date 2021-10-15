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

    Assert(expression != NULL, "[parser.c] - [Parser_Analyze] - Nao foi possivel alocar memoria!\n");

    Token token;
    bool isFloatExpression = false;

    token = Lexer_NextToken();
    Lexer_CheckToken(token);

    if(token.type != TOKEN_SYMBOL_OPENPAR){
        fprintf(stderr, "Erro de sintaxe: token esperado \'(\'\n");
        exit(1);
    }

    token = Lexer_NextToken();
    Lexer_CheckToken(token);

    if(token.type != TOKEN_INT && token.type != TOKEN_FLOAT){
        fprintf(stderr, "Erro de sintaxe: era esperado um numero\n");
        exit(1);
    }

    isFloatExpression = token.type == TOKEN_FLOAT;

    if(isFloatExpression){
        expression->parameterType = PARAMETER_TYPE_FLOAT;
        expression->parameter0.valueF = token.data.valueF;
    }
    else{
        expression->parameterType = PARAMETER_TYPE_INT;
        expression->parameter0.value = token.data.value;
    }

    token = Lexer_NextToken();
    Lexer_CheckToken(token);

    int operation = TokenTypeToOperation(token.type);

    if(operation == -1){
        fprintf(stderr, "Erro de sintaxe: era esperado um operador\n");
        exit(1);
    }

    expression->operation = operation;

    token = Lexer_NextToken();
    Lexer_CheckToken(token);

    if(token.type != TOKEN_INT && token.type != TOKEN_FLOAT){
        fprintf(stderr, "Erro de sintaxe: era esperado um numero\n");
        exit(1);
    }

    if(isFloatExpression){
        expression->parameter1.valueF = token.type == TOKEN_FLOAT ? token.data.valueF : (double)token.data.value;
    }
    else{
        expression->parameter1.value = token.type == TOKEN_FLOAT ? (long)token.data.valueF : token.data.value;
    }

    token = Lexer_NextToken();
    Lexer_CheckToken(token);

    if(token.type != TOKEN_SYMBOL_CLOSEPAR){
        fprintf(stderr, "Erro de sintaxe: token esperado \')\'\n");
        exit(1);
    }

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

void Parser_ExecuteProgram(Program program){
    Expression *expression = program.expression;
    if(expression->parameterType == PARAMETER_TYPE_FLOAT){

        switch (expression->operation)
        {
            case OPERATION_PLUS:
            printf("%lf\n", expression->parameter0.valueF + expression->parameter1.valueF);
            break;

            case OPERATION_MULT:
            printf("%lf\n", expression->parameter0.valueF * expression->parameter1.valueF);
            break;

            case OPERATION_DIV:
            printf("%lf\n", expression->parameter0.valueF / expression->parameter1.valueF);
            break;

            case OPERATION_MINUS:
            printf("%lf\n", expression->parameter0.valueF - expression->parameter1.valueF);
            break;

            default:
            Assert(false, "[parser.c] - [Parser_ExecuteProgram] - Operacao nao identificada!");
        }
    }
    else{

        switch (expression->operation)
        {
            case OPERATION_PLUS:
            printf("%ld\n", expression->parameter0.value + expression->parameter1.value);
            break;

            case OPERATION_MULT:
            printf("%ld\n", expression->parameter0.value * expression->parameter1.value);
            break;

            case OPERATION_DIV:
            printf("%ld\n", expression->parameter0.value / expression->parameter1.value);
            break;

            case OPERATION_MINUS:
            printf("%ld\n", expression->parameter0.value - expression->parameter1.value);
            break;

            default:
            Assert(false, "[parser.c] - [Parser_ExecuteProgram] - Operacao nao identificada!");
        }
    }
}

void Parser_DestroyProgram(Program program){
    free(program.expression);
}