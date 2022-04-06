#pragma once

#include"common_types.h"
#include"core.h"

ulong djb2(uchar* str);

ulong sdbm(uchar* str);

ulong hash_i(uchar* str,uint i);

// djb2 for Map
ulong hash_string(String str);

uint hash_int(Pointer value);
