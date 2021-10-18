 #include "number.h"
 #include <stdio.h>

 Number Number_Add(Number a, Number b){
    
    Number result;

    if(a.isFloat || b.isFloat){ 
        result.isFloat = true;
        result.data.valueFloat = (a.isFloat ? a.data.valueFloat : (double)a.data.valueInt) + 
                                 (b.isFloat ? b.data.valueFloat : (double)b.data.valueInt);
    }
    else{
        result.isFloat = false;
        result.data.valueInt = a.data.valueInt + b.data.valueInt;
    }

    return result;
}

 Number Number_Minus(Number a, Number b){
    
    Number result;

    if(a.isFloat || b.isFloat){ 
        result.isFloat = true;
        result.data.valueFloat = (a.isFloat ? a.data.valueFloat : (double)a.data.valueInt) - 
                                 (b.isFloat ? b.data.valueFloat : (double)b.data.valueInt);
    }
    else{
        result.isFloat = false;
        result.data.valueInt = a.data.valueInt - b.data.valueInt;
    }

    return result;
}

 Number Number_Mult(Number a, Number b){
    
    Number result;

    if(a.isFloat || b.isFloat){ 
        result.isFloat = true;
        result.data.valueFloat = (a.isFloat ? a.data.valueFloat : (double)a.data.valueInt) * 
                                 (b.isFloat ? b.data.valueFloat : (double)b.data.valueInt);
    }
    else{
        result.isFloat = false;
        result.data.valueInt = a.data.valueInt * b.data.valueInt;
    }

    return result;
}

 Number Number_Div(Number a, Number b){
    
    Number result;

    if(a.isFloat || b.isFloat){ 
        result.isFloat = true;
        result.data.valueFloat = (a.isFloat ? a.data.valueFloat : (double)a.data.valueInt) / 
                                 (b.isFloat ? b.data.valueFloat : (double)b.data.valueInt);
    }
    else{
        result.isFloat = false;
        result.data.valueInt = a.data.valueInt / b.data.valueInt;
    }

    return result;
}

 void Number_Println(Number number){
    if(number.isFloat){
        printf("%lf\n", number.data.valueFloat);
    }else{
        printf("%ld\n", number.data.valueInt);
    }
}
