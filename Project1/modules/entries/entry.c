#include"../../include/entry.h"

void create_entry(const String word, Entry entry){
    // Allocate memory for the entry
    entry = (Entry)malloc(sizeof(entry));
    // Set word
    entry->word = strdup(word);
    // Set payload to NULL for Project1
    entry->payload = NULL;
    // Set next to NULL (entries also function as Entry List nodes)
    entry->next = NULL;
}

void destroy_entry(Entry entry){
    // Free the word
    free(entry->word);
    // Free the entry
    free(entry);
}

void create_entry_list(EntryList el){
    
    el = (EntryList)malloc(sizeof(entry_list));
    
    el->head = NULL;
    el->end = NULL;

}

unsigned int get_number_entries(const EntryList el){

    Entry temp = el->head;

    unsigned int number = 0;

    while(temp!=NULL){
        number++;
        temp = temp->next;
    }

    return number;

}

void add_entry(EntryList el, const Entry e){
    el->end->next = e;
}

Entry get_first(const EntryList el){
    if(el->head!=NULL)  return el->head;
    else return NULL;
}

Entry get_next(const EntryList el, const Entry e){
    if(e->next!=NULL) return e->next;
    else return NULL;
}

void destroy_list_nodes(Entry e){
    if(e==NULL) return;
    destroy_list_nodes(e->next);
    destroy_entry(e);
}

void destroy_entry_list(EntryList el){
    destroy_list_nodes(get_first(el));
    free(el);
}