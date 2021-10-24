#pragma once

#include"common_types.h"

struct word{
    String first_letter;
    String word;
};

// Create a word
Word word_create(char first_letter,String whole_word);

// Destroy a word
void word_destroy(Word word);
