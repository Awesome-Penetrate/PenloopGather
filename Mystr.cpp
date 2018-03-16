//
// Created by payloads on 18-3-11.
//

#include "Mystr.h"

bool Mystr::_checkStr(char *str, size_t length) {
    for (int i = 0; i < length; ++i) {
        if(str[i] == '\0'){
            break;
        }
        if(int(str[i]) == 39){
            return false;
        }
    }
    return true;
    
}

bool Mystr::_replaceStr(char *str, size_t length) {
    for (int i = 0; i < length; ++i) {
        if(str[i] == '\0'){
            break;
        }
        if(int(str[i]) == 39){
            str[i] = '*';
        }
    }
    return true;
}