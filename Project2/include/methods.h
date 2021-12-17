#pragma once

#include"common_types.h"
#include"word.h"
#include"bst.h"
#include"map.h"
#include"dictionary.h"
#include"hash_functions.h"
#include"core.h"
#include"query_map.h"
#include"documents.h"


struct core{
    Map document;
    Dictionary exact_queries; //A hashtable that stores the queries with exact match type( buckets are entry lists)
    Dictionary edit_queries; //A hashtable that stores the queries with edit match type( buckets are entry lists)
    Dictionary hamming_queries; //A hashtable that stores the queries with hamming match type( buckets are entry lists)
    uint active_queries_number;
    QueryMap query_map;         // Active Query Set | Here we store the active queries so their info (struct query) is more easily accessible
    QueryList th_boxes[4];      //This is an array that holds a list of queries for every possible match_dist (0,1,2,3)
    DocumentPtr* docs;
    uint document_number;
};

// Global struct to store all the needed structs
Core core_create();

// Destroy the global struct
void core_destroy(Core core);

// Remove special characters and decapitalize words
void remove_special_characters_decapitalize(String word);

// Parse a .txt file
// No duplicate words or special characters allowed
int deduplication(FILE* document,Map map);

// Tokenize a String and don't allow duplicates to be inserted at the Map
int dedup(const char* string, Core core);

// Check whether a word already exists at a Map
bool exact_matching(Map map,String word);

// Check whether a word has already matched a query, if not,match it (check modules/methods/matchquery.c)
void matchQuery(Query query, String word, DocumentPtr doc);

// Add a query id to the results of a doc
void add_query_to_doc_results(QueryID query_id, DocumentPtr doc);
