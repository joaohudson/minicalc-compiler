#ifndef _LEXER_H_
#define _LEXER_H_

typedef enum{
    TOKEN_ERROR,
    TOKEN_EOF,
    TOKEN_PRINT,
    TOKEN_INT,
    TOKEN_SYMBOL_PLUS,
    TOKEN_SYMBOL_MINUS,
    TOKEN_SYMBOL_MULT,
    TOKEN_SYMBOL_DIV,
    TOKEN_SYMBOL_OPENPAR,
    TOKEN_SYMBOL_CLOSEPAR,
    TOKEN_SYMBOL_OPENBRA,
    TOKEN_SYMBOL_CLOSEBRA
}TokenType;

typedef struct
{
    TokenType type;
    union{
        long value;
        char* message;
    } data;
}Token;

void Lexer_Init(const char* fileName);

Token Lexer_NextToken();

void Lexer_PrintToken(Token token);

void Lexer_Quit();

#endif