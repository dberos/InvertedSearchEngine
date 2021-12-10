#pragma once

#include"common_types.h"
#include"hash_functions.h"
#include"list.h"
#include"core.h"


struct dictionary_node{
    Pointer key;
    List value;
};

struct dictionary{
    DictionaryNode array;
    int size;
    int capacity;
    HashFunc hash_function;
};

// Creating a Dictionary
Dictionary dictionary_create();

// Destroying a Dictionary
void dictionary_destroy(Dictionary dictionary);

// Inserting at a Dictionary
void dictionary_insert(Dictionary dictionary,Pointer key,Pointer value);

// Rehashing Depending on Load Factor
void dictionary_rehash(Dictionary dictionary);

// Find whether a key exists at a dictionary and return the List of the values
List dictionary_find(Dictionary dictionary,Pointer key);
