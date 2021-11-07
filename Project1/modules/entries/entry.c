#include"../../include/entry.h"

Entry entry_create(String word){
    // Allocate memory for the entry
    Entry entry=malloc(sizeof(*entry));
    // Set word
    entry->word=strdup(word);
    // Set payload to NULL for Project1
    entry->payload=NULL;
    // Return the entry
    return entry;
}

void entry_destroy(Entry entry){
    // Free the word
    free(entry->word);
    // Free the entry
    free(entry);
}
