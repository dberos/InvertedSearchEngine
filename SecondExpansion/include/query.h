#pragma once

#include"common_types.h"
#include"core.h"
#include"methods.h"
#include"linked_list.h"

///////////////////////////////////////////////////////////////////////////////////////////////

// Keeps all information related to an active query
struct Query{
	QueryID query_id;
	MatchType match_type;
	atomic_int match_dist;
	LinkedList list;
};

///////////////////////////////////////////////////////////////////////////////////////////////

// Creating a Query
Query query_create(QueryID id, MatchType match_type, uint match_dist);

// Add word in the array we are storing query's words
void addWord_to_query(Query query, String word);

// Pass the words of a query to another one ( is used in query_map rehash function)
void passWords_to_query(Query new_query, Query old_query);

// Destroying a Query
void query_destroy(Query query);

// Cleanup each of Querys's words and add them to its array
void query_cleanup(Query query,String query_str);
