#include"../../include/methods.h"

bool exact_matching(Map map,String word){
    bool exists=map_find(map,word);
    return exists;
}