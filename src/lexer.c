#include <string.h>
#include <stdlib.h>
#include <ctype.h>
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

        Assert(tokenBuffer != NULL, "Erro ao alocar memoria!\n");
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

Token Lexer_NextToken(){
    Token token = {
        TOKEN_ERROR,
        0
    };

    while(position < bufferSize && isspace(buffer[position]))
        ++position;


    if(position >= bufferSize){
        token.type = TOKEN_EOF;
    }
    else if(isdigit(buffer[position])){
        
        int begin = position;

        while(position < bufferSize && isdigit(buffer[position]))
            ++position;

        int length = position - begin;

        Reserve(length + 1);//1 slot extra para o '\0'
        memcpy(tokenBuffer, buffer + begin, length);
        tokenBuffer[length] = '\0';//length = tokenBufferSize - 1 = último slot do buffer

        token.type = TOKEN_INT;
        token.data.value = atoi(tokenBuffer);

    }else if(isalpha(buffer[position])){
        int begin = position;

        while(position < bufferSize && !isspace(buffer[position]))
            ++position;

        int length = position - begin;

        Reserve(length + 1);//1 slot extra para o '\0'
        memcpy(tokenBuffer, buffer + begin, length);
        tokenBuffer[length] = '\0';//length = tokenBufferSize - 1 = último slot do buffer

        if(!strcmp(tokenBuffer, "print")){
            token.type = TOKEN_PRINT;
        }
        else{
            token.data.message = "Token inesperado!";
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

            default:
                token.type = TOKEN_ERROR;
                token.data.message = "Caractere invalido!";
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
        printf("TOKEN_INT: %d\n", token.data.value);
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
    
    default:
        printf("Token nao reconhecido!\n");
    }
}