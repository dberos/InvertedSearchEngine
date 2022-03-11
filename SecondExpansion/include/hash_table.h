#pragma once

#include"common_types.h"
#include"core.h"
#include"query.h"
#include"linked_list.h"
#include"hash_functions.h"

// Open Addressing Hash Table to match existing Queries 

struct hash_node{
    String key;                 // Key is the QueryID
    LinkedList value;           // Value is a List with all matched words of this Query
};

struct hash_table{
    HashNode array;             // Array of Hash Table
    HashFunction hash_function; // Hash Fucntion
    int size;                   // Size of Hash Table
    int capacity;               // Capacity of Hash Table
};

// Creating a Hash Table
HashTable hash_table_create();

// Destroying a Hash Table
void hash_table_destroy(HashTable hash_table);

/** 
 * Inserting a QueryID at a Hash Table
 * @return true --> Bucket's linked_list->size == num_words (Matched)
 **/
bool hash_table_insert(HashTable hash_table,QueryID query_id,String word,int num_words);

// Rehashing depending on Load Factor
void hash_table_rehash(HashTable hash_table);

// Check whether QueryID has already matched
bool hash_table_find(HashTable hash_table,QueryID query_id,int num_words);
