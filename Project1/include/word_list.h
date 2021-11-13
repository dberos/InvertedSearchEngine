#pragma once

#include"common_types.h"

typedef struct word_node{
    String word;
    struct word* next;
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
