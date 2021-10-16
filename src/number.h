#ifndef _NUMBER_H_
#define _NUMBER_H_

#include <stdbool.h>

typedef struct {
    bool isFloat;
    union{
        long valueInt;
        double valueFloat;
    }data;
}Number;

Number Number_Add(Number a, Number b);

Number Number_Minus(Number a, Number b);

Number Number_Mult(Number a, Number b);

Number Number_Div(Number a, Number b);

void Number_Println(Number number);

#endif