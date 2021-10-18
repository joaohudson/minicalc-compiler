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
    TokenType closeTokenType;//tipo do token de fechamento, ')' ou ']' deve ser igual ao de abertura
    char closeTokenChar, openTokenChar;

    token = Lexer_NextToken();
    Lexer_CheckToken(token);

    if(token.type == TOKEN_INT){
        expression->operation = OPERATION_CONST;
        expression->value.isFloat = false;
        expression->value.data.valueInt = token.data.valueInt;
        return expression;
    }

    if(token.type == TOKEN_FLOAT){
        expression->operation = OPERATION_CONST;
        expression->value.isFloat = true;
        expression->value.data.valueFloat = token.data.valueFloat;
        return expression;
    }

    if(token.type == TOKEN_SYMBOL_OPENPAR){
        closeTokenType = TOKEN_SYMBOL_CLOSEPAR;
        closeTokenChar = ')';
        openTokenChar = '(';
    }
    else{
        closeTokenType = TOKEN_SYMBOL_CLOSEBRA;
        closeTokenChar = ']';
        openTokenChar = '[';        
    }
     

    if(token.type != TOKEN_SYMBOL_OPENPAR && token.type != TOKEN_SYMBOL_OPENBRA){
        fprintf(stderr, "Erro de sintaxe: token esperado \'%c\'\n", openTokenChar);
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

    if(token.type != closeTokenType){
        fprintf(stderr, "Erro de sintaxe: token esperado \'%c\'\n", closeTokenChar);
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

void Parser_PrintExpression(Expression *expression){
    if(expression->operation == OPERATION_CONST){
        Number_Println(expression->value);
        return;
    }
    Parser_PrintExpression(expression->parameter0);
    Parser_PrintExpression(expression->parameter1);
}

static Number ComputeExpression(Expression *expression){
    Number result, a, b;

    switch(expression->operation){
        case OPERATION_CONST:
            result = expression->value;
        break;

        case OPERATION_PLUS:
            a = ComputeExpression(expression->parameter0);
            b = ComputeExpression(expression->parameter1);
            result = Number_Add(a, b);
        break;

        case OPERATION_MINUS:
            a = ComputeExpression(expression->parameter0);
            b = ComputeExpression(expression->parameter1);
            result = Number_Minus(a, b);
        break;

        case OPERATION_MULT:
            a = ComputeExpression(expression->parameter0);
            b = ComputeExpression(expression->parameter1);
            result = Number_Mult(a, b);
        break;

        case OPERATION_DIV:
            a = ComputeExpression(expression->parameter0);
            b = ComputeExpression(expression->parameter1);
            result = Number_Div(a, b);
        break;
    }

    return result;
}

void Parser_ExecuteProgram(Program program){
    Number number = ComputeExpression(program.expression);
    Number_Println(number);
}

static void DestroyExpression(Expression *expression){
    if(expression == NULL)
        return;

    DestroyExpression(expression->parameter0);
    DestroyExpression(expression->parameter1);
    free(expression);
}

void Parser_DestroyProgram(Program program){
    DestroyExpression(program.expression);
}
