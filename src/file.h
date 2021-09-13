#ifndef _FILE_H_
#define _FILE_H_

#include <stdio.h>

void Assert(int cond, const char* error);

/**
 * Informa o comprimento do arquivo.
 */
long File_Length(FILE *file);

/**
 * Ler todo o texto do arquivo.
 */ 
char* File_ReadAllText(const char* fileName);

#endif