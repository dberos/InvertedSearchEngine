#pragma once

#include"common_types.h"
#include"list.h"
#include"core.h"

struct entry{
    String word;
    List payload;
    Entry next;
};


struct entry_list{
    Entry head;
    Entry tail;
    atomic_int size;
};

// Creating an Entry
Entry create_entry(String word);

// Destroying an Entry
void destroy_entry(Entry entry,EntryList entry_list);

// Creating an entry list
EntryList create_entry_list();

// Return size
uint get_number_entries(EntryList entry_list);

// Add entry to an Entry List
void add_entry(EntryList entry_list,Entry entry);

// Get first entry of the Entry List
Entry get_first(EntryList entry_list);

// Get next Entry
Entry get_next(EntryList entry_list,Entry entry);

// Destroying an entry list
void destroy_entry_list(EntryList entry_list);

// Insert a word at an Entry List
// If it doesnt exist create an entry 
// Else update the payload
bool insert_entry(EntryList entry_list,String word,uint id);
