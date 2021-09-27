#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"

int main(void){

    Lexer_Init("fonte.mc");

    Program program = Parser_Analyze();

    Parser_ExecuteProgram(program);

    Parser_DestroyProgram(program);

    Lexer_Quit();

    return 0;
}