#pragma once

#include"common_types.h"
#include"word.h"
#include"bst.h"
#include"hash_functions.h"

struct map_node{
    String key;
    Bst bst;
    State state;
};

struct map{
    MapNode array;
    int size;
    int capacity;
    HashFunction hash_function;
};

// Creating a Map
Map map_create();

// Destroying a Map
void map_destroy(Map map);

// Inserting at Map a word, with key the first letter and whole word at the Node's Bst
void map_insert(Map map,Word word);

// Rehash the Map depending on Load Factor
void map_rehash(Map map);

// Find whether a key exists at a Map
bool map_check_key(Map map,Word word);

// For a Map that contains in a Bst "Aa" etc to convert from capital letters and not use ctype.h and tolower()
// e.h Deduplication and deduplication should count as 1 word in parsing
String map_return_key(Map map,Word word);
