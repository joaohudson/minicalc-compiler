#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

int main(void){

    Lexer_Init("fonte.mc");

    Token token;

    do{
        token = Lexer_NextToken();
        Lexer_PrintToken(token);

        if(token.type == TOKEN_ERROR){
            fprintf(stderr, "%s\n", token.data.message);
            break;
        }

    }while(token.type != TOKEN_EOF);

    Lexer_Quit();

    return 0;
}