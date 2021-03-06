#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "file.h"
#include "lexer.h"

static char* buffer = NULL;
static char* tokenBuffer = NULL;
static size_t tokenBufferSize = 0;
static size_t bufferSize = 0;
static int position = 0;

/**
 * Reserva espaço no tokenBuffer para se ajustar
 * a string desejada
 */
static void Reserve(size_t size){
    if(size > tokenBufferSize){
        free(tokenBuffer);
        tokenBuffer = malloc(size);
        tokenBufferSize = size;

        Assert(tokenBuffer != NULL, "[lexer.c] - [Reserve] - Erro ao alocar memoria!\n");
    }
}

void Lexer_Init(const char* fileName){
    buffer = File_ReadAllText(fileName);
    bufferSize = strlen(buffer);
}

void Lexer_Quit(){
    free(tokenBuffer);
    free(buffer);
}

void Lexer_CheckToken(Token token){
    if(token.type == TOKEN_ERROR){
        fprintf(stderr, "Erro lexico: %s\n", token.data.message);
        exit(1);
    }
}

Token Lexer_NextToken(){
    Token token;

    while(position < bufferSize && isspace(buffer[position]))
        ++position;

    while(position < bufferSize && buffer[position] == '#'){
        while(position < bufferSize){
            ++position;

            if(buffer[position] == '\n'){
                ++position;
                break;
            }
        }
    }

    if(position >= bufferSize){
        token.type = TOKEN_EOF;
    }
    else if(isdigit(buffer[position])){
        
        int begin = position;
        bool isFloat = false;

        while(position < bufferSize && isdigit(buffer[position])){
            ++position;

            if(buffer[position] == '.' && !isFloat){
                isFloat = true;
                ++position;
            }
        }

        int length = position - begin;

        Reserve(length + 1);//1 slot extra para o '\0'
        memcpy(tokenBuffer, buffer + begin, length);
        tokenBuffer[length] = '\0';//length = tokenBufferSize - 1 = último slot do buffer

        if(isFloat){
            token.type = TOKEN_FLOAT;
            token.data.valueFloat = atof(tokenBuffer);
        }
        else{
            token.type = TOKEN_INT;
            token.data.valueInt = atoi(tokenBuffer);
        }

    }else if(isalpha(buffer[position])){
        int begin = position;

        while(position < bufferSize && !isspace(buffer[position]) && (isalpha(buffer[position]) || isdigit(buffer[position]) || buffer[position] == '_') )
            ++position;

        int length = position - begin;

        Reserve(length + 1);//1 slot extra para o '\0'
        memcpy(tokenBuffer, buffer + begin, length);
        tokenBuffer[length] = '\0';//length = tokenBufferSize - 1 = último slot do buffer

        if(!strcmp(tokenBuffer, "print")){
            token.type = TOKEN_PRINT;
        }
        else if(!strcmp(tokenBuffer, "var")){
            token.type = TOKEN_VAR;
        }
        else if(strlen(tokenBuffer) < VALUES_IDENTIFIER_CAPACITY){
            token.type = TOKEN_IDENTIFIER;
            strcpy(token.data.identifier, tokenBuffer);
        }
        else{
            token.type = TOKEN_ERROR;
            token.data.message = "Tamanho da variavel excede o limite maximo";
        }
    }
    else{
        switch(buffer[position]){
            case '(':
                token.type = TOKEN_SYMBOL_OPENPAR;
                break;

            case ')':
                token.type = TOKEN_SYMBOL_CLOSEPAR;
                break;

            case '+':
                token.type = TOKEN_SYMBOL_PLUS;
                break;

            case '-':
                token.type = TOKEN_SYMBOL_MINUS;
                break;

            case '*':
                token.type = TOKEN_SYMBOL_MULT;
                break;

            case '/':
                token.type = TOKEN_SYMBOL_DIV;
                break;

            case '[':
                token.type = TOKEN_SYMBOL_OPENBRA;
                break;

            case ']':
                token.type = TOKEN_SYMBOL_CLOSEBRA;
                break;

            case '=':
                token.type = TOKEN_SYMBOL_EQUALS;
                break;

            case ';':
                token.type = TOKEN_SYMBOL_SEMICOLON;
                break;

            default:
                token.type = TOKEN_ERROR;
                token.data.message = "Token invalido!";
        }

        position++;
    }

    return token;
}

void Lexer_PrintToken(Token token){
    switch (token.type)
    {
    case TOKEN_EOF:
        printf("EOF\n");
        break;

    case TOKEN_ERROR:
        printf("TOKEN_ERROR\n");
        break;

    case TOKEN_PRINT:
        printf("TOKEN_PRINT\n");
        break;

    case TOKEN_INT:
        printf("TOKEN_INT: %ld\n", token.data.valueInt);
        break;

    case TOKEN_SYMBOL_DIV:
        printf("TOKEN_SYMBOL_DIV\n");
        break;

    case TOKEN_SYMBOL_MULT:
        printf("TOKEN_SYMBOL_MULT\n");
        break;

    case TOKEN_SYMBOL_MINUS:
        printf("TOKEN_SYMBOL_MINUS\n");
        break;

    case TOKEN_SYMBOL_PLUS:
        printf("TOKEN_SYMBOL_PLUS\n");
        break;

    case TOKEN_SYMBOL_OPENPAR:
        printf("TOKEN_SYMBOL_OPENPAR\n");
        break;

    case TOKEN_SYMBOL_CLOSEPAR:
        printf("TOKEN_SYMBOL_CLOSEPAR\n");
        break;

    case TOKEN_SYMBOL_OPENBRA:
        printf("TOKEN_SYMBOL_OPENBRA\n");
        break;

    case TOKEN_SYMBOL_CLOSEBRA:
        printf("TOKEN_SYMBOL_CLOSEBRA\n");
        break;

    case TOKEN_FLOAT:
        printf("TOKEN_FLOAT: %lf\n", token.data.valueFloat);
        break;

    case TOKEN_VAR:
        printf("TOKEN_VAR\n");
        break;

    case TOKEN_SYMBOL_EQUALS:
        printf("TOKEN_SYMBOL_EQUALS\n");
        break;

    case TOKEN_SYMBOL_SEMICOLON:
        printf("TOKEN_SYMBOL_SEMICOLON\n");
        break;

    case TOKEN_IDENTIFIER:
        printf("TOKEN_IDENTIFIER: %s\n", token.data.identifier);
        break;
    
    default:
        printf("Token nao reconhecido: %d\n", token.type);
    }
}