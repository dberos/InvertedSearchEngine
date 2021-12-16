#pragma once

#include"common_types.h"
#include"core.h"
#include"query.h"
#include"query_list.h"
#include"hash_functions.h"

// It is the active query set
// Better from a Vector in case query ids come randomly like 1,2,876..
// So VectorNode[2] would have QueryID 876 and it is not useful

struct query_map_node{
    QueryList query_list; 
};

struct query_map{
    QueryMapNode array;
    int size;
    int capacity;
    HashFunction hash_function;
};

// Creating an active Query Set
QueryMap query_map_create();

// Destroying an active Query Set
void query_map_destroy(QueryMap query_map);

// Inserting a Query at an active Query Set
void query_map_insert(QueryMap query_map, Query query);

// Rehashing depending on Load Factor
void query_map_rehash(QueryMap query_map);

// Remove a Query from the active query set based on its id
void query_map_remove(QueryMap query_map,Pointer id);

// Find the Query of given id
Query query_map_find(QueryMap query_map,Pointer id);
