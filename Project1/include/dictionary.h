#pragma once

#include"common_types.h"
#include"word.h"
#include"word_list.h"
#include"hash_functions.h"

// It is only used inside deduplication just to make things easier when trying to change capital to lowcase letter
// Not the basic struct of the exercise since entry list nodes should contain one entry and not e.g a bst of words with the same initial 

struct dictionary{
    WordList* array;
    int size;
    int capacity;
    HashFunction hash_function;
};

// Creating a Dictionary
Dictionary dictionary_create();

// Destroying a Dictionary
void dictionary_destroy(Dictionary dictionary);

// Inserting at a Dictionary
void dictionary_insert(Dictionary dictionary,Word word);

// Rehashing Depending on Load Factor
void dictionary_rehash(Dictionary dictionary);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////        Helpful functions only to be used inside deduplication      ////////////////////

// Initialize a Dictionary with capital and its lowercase letter
void dictionary_init(Dictionary dictionary);

// Check whether a capital letter is present
bool dictionary_check_key(Dictionary dictionary,Word word);

// Return the first word if it exists
// Only for dictionary that contains alphabet e.g Aa,Bb..
String dictionary_return_key(Dictionary dictionary,Word word);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
