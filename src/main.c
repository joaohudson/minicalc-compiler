#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "file.h"

int main(int argc, char**argv){

    if(argc < 2){
        fprintf(stderr, "Erro: informe o nome do programa!\n");
        exit(1);
    }

    if(!File_Exists(argv[1])){
        fprintf(stderr, "Erro: informe um nome de um programa existente!\n");
        exit(1);
    }

    Lexer_Init(argv[1]);

    Program program = Parser_Analyze();

    Parser_PrintExpression(program);
    //Parser_ExecuteProgram(program);

    Parser_DestroyProgram(program);

    Lexer_Quit();

    return 0;
}