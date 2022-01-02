#include"../../include/hash_functions.h"

ulong hash_string(String str){
    ulong hash = 5381;
    int c; 
    while ((c = *str++)){
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

uint hash_id(Pointer value){
    return *(uint*)value;
}
