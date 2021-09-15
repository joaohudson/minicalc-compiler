#include <stdlib.h>
#include <string.h>
#include "file.h"

void Assert(int cond, const char* error){
    if(!cond){
        fprintf(stderr, "%s\n", error);
        exit(1);
    }
}

size_t FileLength(FILE *file){
    int i = 0;

    while(getc(file) != EOF)
        i++;

    rewind(file);

    return i;
}

char* File_ReadAllText(const char* fileName){
    FILE *file = fopen(fileName, "r");

    Assert(file != NULL, "[file.c] - [File_ReadAllText] - Nao foi possivel abrir o arquivo!");

    long len = FileLength(file);
    char *data = (char*)malloc(len + 1);
    
    fread(data, 1, len, file);
    data[len] = '\0';//adiciona terminal da string

    fclose(file);

    return data;
}