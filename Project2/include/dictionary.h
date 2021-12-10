#pragma once

#include"common_types.h"
#include"hash_functions.h"
#include"list.h"
#include"core.h"
#include"entry.h"


struct dictionary_node{
    EntryList entry_list;
};

struct dictionary{
    DictionaryNode array;
    int size;
    int capacity;
    HashFunction hash_function;
};

// Creating a Dictionary
Dictionary dictionary_create();

// Destroying a Dictionary
void dictionary_destroy(Dictionary dictionary);

// Inserting at a Dictionary
bool dictionary_insert(Dictionary dictionary,String word,Pointer id);

// Rehashing Depending on Load Factor
void dictionary_rehash(Dictionary dictionary);

// Find whether an entry exists at a dictionary and return it
Entry dictionary_find(Dictionary dictionary,String word);

