#pragma once

#include"common_types.h"
#include"word.h"

// Only used for the helpful dictionary for the deduplication

struct word_list_node{
    Word word;
    WordListNode next;
};

struct word_list{
    WordListNode head;
    int size;
};

// Creating a List to store words(capital,whole word)
WordList word_list_create();

// Destroying a Word List
void word_list_destroy(WordList list);

// Inserting at the Word List
void list_insert(WordList list,String word);

// Inserting at the end of a Word List to not allow duplicates
bool word_list_insert_end(WordList list,String word);
