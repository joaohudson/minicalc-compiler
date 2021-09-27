#ifndef _FILE_H_
#define _FILE_H_

#include <stdio.h>
#include <stdbool.h>

void Assert(int cond, const char* error);

/**
 * Ler todo o texto do arquivo.
 */ 
char* File_ReadAllText(const char* fileName);

bool File_Exists(const char* fileName);

#endif