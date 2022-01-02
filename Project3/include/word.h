#pragma once

#include"common_types.h"

// Only used to make things easier for deduplication
// It won't be part of the entry struct

struct word{
    String first_letter;
    String word;
};

// Create a word
Word word_create(String string);

// Destroy a word
void word_destroy(Word word);
