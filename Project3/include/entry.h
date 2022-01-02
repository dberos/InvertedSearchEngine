#pragma once

#include"common_types.h"
#include"list.h"
#include"core.h"

struct entry{
    String word;
    List payload;
    Entry next;
} entry;


struct entry_list{
    Entry head;
    Entry end;
    uint size;
} entry_list;

// Creating an Entry
Entry create_entry(String word);
Entry create_entry_with_payload(const String word, List payload);

// Destroying an Entry
void destroy_entry(Entry entry,EntryList entry_list);

//Creating and entry list
EntryList create_entry_list();

//Count length of entry list
unsigned int get_number_entries(const EntryList el);

//Add entry to entry lsit
void add_entry(EntryList el, const Entry e);

//Get first entry of the entrylist
Entry get_first(const EntryList el);

//Get next entry of the list(given the previous entry)
Entry get_next(const EntryList el, const Entry e);

//Destroy entry list
void destroy_list_nodes(Entry e,EntryList entry_list); //Recursive function used in destroy_entry_list
void destroy_entry_list(EntryList el);

// Insert a word at an Entry List
// If it doesnt exist create an entry 
// Else update the payload
bool insert_entry(EntryList entry_list,String word,uint id);


void print_payload(List payload);
