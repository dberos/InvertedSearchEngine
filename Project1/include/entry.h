#pragma once

#include"common_types.h"

struct entry{
    String word;
    String payload;
    Entry next;
} entry;


struct entry_list{
    Entry head;
    Entry end;
} entry_list;

// Creating an Entry
void create_entry(String word, Entry entry);

// Destroying an Entry
void destroy_entry(Entry entry);

//Creating and entry list
void create_entry_list(EntryList el);

//Count length of entry list
unsigned int get_number_entries(const EntryList el);

//Add entry to entry lsit
void add_entry(EntryList el, const Entry e);

//Get first entry of the entrylist
Entry get_first(const EntryList el);

//Get next entry of the list(given the previous entry)
Entry get_next(const EntryList el, const Entry e);

//Destroy entry list
void destroy_list_nodes(Entry e); //Recursive function used in destroy_entry_list
void destroy_entry_list(EntryList el);


