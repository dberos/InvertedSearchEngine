#include"../../include/word.h"

Word word_create(String string){
    // Allocate memory for the word
    Word word=malloc(sizeof(*word));
    // First letter created as a String and not char
    String first=malloc(2);
    sprintf(first,"%c",string[0]);
    word->first_letter=strdup(first);
    // Whole word
    word->word=strdup(string);
    free(first);
    return word;
}

void word_destroy(Word word){
    free(word->first_letter);
    free(word->word);
    free(word);
}
