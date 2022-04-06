#include"../../include/hash_functions.h"

// Used double parentheses on both whiles to make compile with Wall
ulong djb2(uchar* str) {
	ulong hash = 5381;
	int c; 
	while ((c = *str++)) {
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	return hash;
}

ulong sdbm(uchar* str) {
	ulong hash = 0;
	int c;

	while ((c = *str++)) {
		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

ulong hash_i(uchar* str,uint i) {
	return djb2(str) + i*sdbm(str) + i*i;
}

ulong hash_string(String str){
    ulong hash = 5381;
    int c; 
    while ((c = *str++)){
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

uint hash_int(Pointer value){
	return *(int*)value;
}
