#pragma once

#include"common_types.h"
#include"hash_functions.h"
#include"list.h"
#include"core.h"
#include"entry.h"
#include"query_map.h"


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
bool dictionary_insert(Dictionary dictionary,String word,uint id);

// Rehashing Depending on Load Factor
void dictionary_rehash(Dictionary dictionary);

// Find whether an entry exists at a dictionary and return it
Entry dictionary_find(Dictionary dictionary,String word);

// Remove a QueryID from the payload of an Entry
void dictionary_remove(Dictionary dictionary,QueryMap query_map,uint id);

// Remove an entry from the EntryList of a dictionary node
void remove_entry_from_list(Dictionary dictionary, String word);

