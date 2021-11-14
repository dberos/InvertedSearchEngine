#pragma once

#include"common_types.h"
#include"word.h"

typedef struct word_node{
    String word;
    struct word_node* next;
}word_node;

typedef word_node* word_node_ptr;


typedef struct words_list{
    word_node_ptr head;
    word_node_ptr end;
    int size;
}words_list;

typedef words_list* words_list_ptr;


//create words list
words_list_ptr word_list_create();
//Destroy words list
void destroy_word_list_node(word_node_ptr node);
void word_list_destroy(words_list_ptr list);
//Add a word to the words list
word_node_ptr create_word_node(const String word);
void add_word_to_list(const String word, words_list_ptr list);
//Remove the first node of the word list and get the word
String pop_word_list(words_list_ptr list);


/////////////////////////   Only used at Dictionary     /////////////////////////

struct word_list_node{
    Word word;
    WordListNode next;
};

struct word_list{
    WordListNode head;
    int size;
};

// Creating a List to store words(capital,whole word)
WordList dictionary_word_list_create();

// Destroying a Word List
void dictionary_word_list_destroy(WordList list);

// Inserting at the Word List
void word_list_insert(WordList list,String word);

// Inserting at the end of a Word List to not allow duplicates
bool word_list_insert_end(WordList list,String word);

//////////////////////////////////////////////////////////////////////////////////

