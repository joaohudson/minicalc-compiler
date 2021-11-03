#include "parser.h"
#include "lexer.h"
#include "file.h"
#include <string.h>
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

    if(token.type == TOKEN_IDENTIFIER){
        expression->operation = OPERATION_CONST_IDENTIFIER;
        strcpy(expression->identifier, token.data.identifier);
        return expression;
    }

    if(token.type == TOKEN_INT){
        expression->operation = OPERATION_CONST_LITERAL;
        expression->value.isFloat = false;
        expression->value.data.valueInt = token.data.valueInt;
        return expression;
    }

    if(token.type == TOKEN_FLOAT){
        expression->operation = OPERATION_CONST_LITERAL;
        expression->value.isFloat = true;
        expression->value.data.valueFloat = token.data.valueFloat;
        return expression;
    }

    if(token.type == TOKEN_SYMBOL_OPENPAR){
        closeTokenType = TOKEN_SYMBOL_CLOSEPAR;
        closeTokenChar = ')';
        openTokenChar = '(';
    }
    else if(token.type == TOKEN_SYMBOL_OPENBRA){
        closeTokenType = TOKEN_SYMBOL_CLOSEBRA;
        closeTokenChar = ']';
        openTokenChar = '[';        
    }
    else{
        fprintf(stderr, "Erro de sintaxe: era esperado um valor ou expressao\n");
        exit(1);
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

static VariableDeclaration* VariableDeclarationAnalyze(){
    VariableDeclaration *variable = malloc(sizeof(VariableDeclaration));
    variable->next = NULL;

    Assert(variable != NULL, "[parser.c] - [VariableDeclarationAnalyze] - Erro ao alocar memoria");

    Token token = Lexer_NextToken();
    Lexer_CheckToken(token);

    if(token.type != TOKEN_IDENTIFIER){
        fprintf(stderr, "Erro de sintaxe: era esperado um identificar apos a palavra chave \'var\'\n");
        exit(1);
    }

    strcpy(variable->identifier, token.data.identifier);

    token = Lexer_NextToken();
    Lexer_CheckToken(token);

    if(token.type != TOKEN_SYMBOL_EQUALS){
        fprintf(stderr, "Erro de sintaxe: token esperado \'=\'\n");
        exit(1);
    }

    variable->expression = ExpressionAnalyze();

    token = Lexer_NextToken();
    Lexer_CheckToken(token);

    if(token.type != TOKEN_SYMBOL_SEMICOLON){
        fprintf(stderr, "Erro de sintaxe: token esperado \';\'\n");
        exit(1);
    }

    return variable;
}

Program Parser_Analyze(){
    Program program;
    Token token;
    VariableDeclaration *current = NULL;

    token = Lexer_NextToken();
    Lexer_CheckToken(token);

    if(token.type == TOKEN_VAR){
        program.variables = VariableDeclarationAnalyze();
        current = program.variables;
        
        do{
            token = Lexer_NextToken();
            Lexer_CheckToken(token);

            if(token.type != TOKEN_VAR){
                break;
            }

            current->next = VariableDeclarationAnalyze();
            current = current->next;

        }while(true);
    }

    if(token.type != TOKEN_PRINT){
        fprintf(stderr, "Erro de sintaxe: palavra chave \'print\' esperada no inicio da expressao\n");
        exit(1);
    }

    program.expression = ExpressionAnalyze();

    return program;
}

void Parser_PrintExpression(Expression *expression){
    if(expression->operation == OPERATION_CONST_LITERAL){
        Number_Println(expression->value);
        return;
    }
    Parser_PrintExpression(expression->parameter0);
    Parser_PrintExpression(expression->parameter1);
}

static Expression* FindExpression(const VariableDeclaration *variables, const char *identifier){
    while(variables){
        if(!strcmp(variables->identifier, identifier))
            return variables->expression;

        variables = variables->next;
    }

    fprintf(stderr, "Erro de sintaxe: variavel nao declarada \'%s\'\n", identifier);
    exit(1);
}

static Number ComputeExpression(Expression *expression, const VariableDeclaration *variables){
    Number result, a, b;

    switch(expression->operation){
        case OPERATION_CONST_LITERAL:
            result = expression->value;
        break;

        case OPERATION_CONST_IDENTIFIER:
            result = ComputeExpression(FindExpression(variables, expression->identifier), variables);
        break;

        case OPERATION_PLUS:
            a = ComputeExpression(expression->parameter0, variables);
            b = ComputeExpression(expression->parameter1, variables);
            result = Number_Add(a, b);
        break;

        case OPERATION_MINUS:
            a = ComputeExpression(expression->parameter0, variables);
            b = ComputeExpression(expression->parameter1, variables);
            result = Number_Minus(a, b);
        break;

        case OPERATION_MULT:
            a = ComputeExpression(expression->parameter0, variables);
            b = ComputeExpression(expression->parameter1, variables);
            result = Number_Mult(a, b);
        break;

        case OPERATION_DIV:
            a = ComputeExpression(expression->parameter0, variables);
            b = ComputeExpression(expression->parameter1, variables);
            result = Number_Div(a, b);
        break;
    }

    return result;
}

void Parser_PrintVariables(VariableDeclaration *variables){
    while(variables){
        printf("Identificador: %s\n", variables->identifier);
        Number number = ComputeExpression(variables->expression, variables);
        printf("Valor: ");
        Number_Println(number);

        variables = variables->next;
    }
}


void Parser_ExecuteProgram(Program program){
    Number number = ComputeExpression(program.expression, program.variables);
    Number_Println(number);
}

static void DestroyExpression(Expression *expression){
    if(expression == NULL)
        return;

    DestroyExpression(expression->parameter0);
    DestroyExpression(expression->parameter1);
    free(expression);
}

static void DestroyVariableDeclarations(VariableDeclaration *variables){
    VariableDeclaration *aux;

    while(variables){
        aux = variables;
        variables = variables->next;
        DestroyExpression(aux->expression);
        free(aux);
    }
}

void Parser_DestroyProgram(Program program){
    DestroyVariableDeclarations(program.variables);
    DestroyExpression(program.expression);
}
