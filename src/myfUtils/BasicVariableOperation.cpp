//
// Created by cuizhou on 18-4-5.
//

#include "BasicVariableOperation.h"

namespace myf{
    int GetIntVariableLength(const int tmp)
    {
        char str[16];
        memset(str, 0, sizeof(str));
        sprintf(str, "%d", tmp);
        return strlen(str);
    }

    string addZeroPrefix(int desiredlength,const int idx){


        int zerolength=desiredlength-GetIntVariableLength(idx);
        if(zerolength<0)return "error";

        stringstream oo;
        while(zerolength>0){
            oo<<"0";
            zerolength--;
        }
        oo<<idx;

        return oo.str();
    }
}